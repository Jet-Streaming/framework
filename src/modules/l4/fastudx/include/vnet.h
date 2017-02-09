#ifndef _LIVECLOUD_RELAY_VNET_H_
#define _LIVECLOUD_RELAY_VNET_H_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string>
#include <list>
#include <utility>
#include "message.h"
/* #include "utlis/compute_time.h" */

#include "FastUdx.h"
#include "common.h"
//#include "util.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

#define PORT(s) (ntohs(s.sin_port))
#define DESTROY_CONN_DELAY_TIMES 1
#define MAX_SID_LEN 128

using namespace common;
inline std::string IP(sockaddr_in s) {
    char buf[64] = {'\0'}; 
    inet_ntop(AF_INET, &s.sin_addr.s_addr, buf, sizeof(buf));
    return string(buf);
}

enum ConnType {
    CONN_UDP = 1,
    CONN_TCP = 2,
};

class VNetCon : public MsgQueue {
protected:
    int _send(const char* buf, int size) {
        if(type == CONN_UDP && (!closed)) {
            if(udx->SendBuff((BYTE*)buf, size) != 0) {
                return size;
            }
            else return udx->IsConnected() ? -1 : 0;
        } else {
            return (int)::send(sa, buf, size, 0);
        }
    }

public:
    char sid[MAX_SID_LEN + 1];        /**< md5 string */
    void* parent;
    ConnType type;
    IUdxTcp* udx;
    ZBuf* packet;
    bool closed;
    bool removed;
    bool remote_closed;
    sockaddr_in sai;
    int sa;
    MsgQueue* ext_queue;						//if set ext_queue we use extend queue to send data so we can reuse send bufs
    unsigned int queue_pos;
    unsigned int ext_pos;
    bool sending;
    VNetCon() {
        sending = false;
        queue_pos = ext_pos = 0;
        packet = NULL;
        closed = removed = remote_closed = false;
        ext_queue = NULL;
        _send_block = NULL;
        _send_pos = _send_size = _head_pos = 0;
        init_sid();
    }

    virtual ~VNetCon() {
        if( NULL != packet) delete packet;
        if( NULL != _send_block) delete _send_block;
    }

    /**
     * @brief 每个连接生成一个唯一标识sid
     * @brief 便于debug，部分类型的连接没有sn，不能用sn标识一个连接的整个流程
     * @warn sid需要主动free, or there will be memory leak
     *
     */
    void init_sid() {
        /*unsigned randseed = urandom();
        char* s = get_random_string(64, &randseed);
        char src_buf[128] = {'0'};
        snprintf(src_buf, sizeof(src_buf) - 1, "%s%s%d%x", s, get_ip(), get_port(), (uint64_t)this);
        char* md5 = get_md5_string(src_buf);
        strncpy(sid, md5, sizeof(sid) / sizeof(char));
        sid[MAX_SID_LEN] = '\0';
        free(s);
        free(md5);
        */
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        string str_sid = boost::lexical_cast<string>(uuid);
        strncpy(sid, str_sid.c_str(), sizeof(sid) / sizeof(char));
        sid[MAX_SID_LEN] = '\0';
    };

    /**
     * @brief protocol to string
     *
     * @param conn
     *
     * @return udp or tcp
     */
    const char* conn_protocol_string() {
        if (type == CONN_TCP)
            return "tcp";
        else if (type == CONN_UDP)
            return "udp";
        else
            return "unknown_protocol";
    }

    virtual void atClearConn() = 0;

    static std::string getAddrString(sockaddr_in sai) {
        std::stringstream ss;
        ss << inet_ntoa(sai.sin_addr) << ':' << ntohs(sai.sin_port);
        return ss.str();
    }

    void remove() { 
        /*if(false == closed) {
          XLOG_FATAL("connection not closed addr[0x%0x] ", (unsigned long)this);
          }
          XLOG_INFO("connection removed addr[0x%0x] ", (unsigned long)this);
          removed = true; 
          */
    }

    static unsigned int getIP(sockaddr_in sai) {
        return sai.sin_addr.s_addr;
    }

    static sockaddr_in getAddr(const char* dns, unsigned short port) {
        struct hostent* hptr = gethostbyname(dns);
        if(hptr != NULL) return getAddr(*(unsigned int*)hptr->h_addr_list[0], port);
        else return getAddr((unsigned int)0, port);
    }

    static sockaddr_in getAddr(unsigned int ip, unsigned short port) {
        sockaddr_in sai;
        sai.sin_family = AF_INET;
        sai.sin_addr.s_addr = ip;
        sai.sin_port = htons(port);
        return sai;
    }


    static sockaddr_in getAddr(const std::string ip, unsigned short port) {
        return getAddr(inet_addr(ip.c_str()), port);
    }

    void set_socket_opt() {
        if(type == CONN_TCP) {
            int flags = fcntl(sa, F_GETFL, 0);
            fcntl(sa, F_SETFL, flags | O_NONBLOCK);
        }
    }

    void close() {
        if (!__sync_bool_compare_and_swap(&closed, false, true)) return;
        if (type == CONN_TCP) {
            if(sa != -1)
                ::close(sa); 
        }
        onClose();
        sa = -1;
        //if(type == CONN_UDP && udx != NULL) udx->Close();
        /*if(type == CONN_UDP && udx != NULL) {
            udx->Close();
            udx = NULL;
        }
        else ::close(sa);
        onClose();
        sa = -1;*/
    }

    bool isBlock() {
        if(type == CONN_UDP) return udx->IsConnected();
        else return errno == EAGAIN || errno == 0;
    }

    bool connect() {
        size_t sailen = sizeof(sai);
        if(type == CONN_UDP) {
            udx->SetUserData((INT64)this);
            return udx->IsConnected();
        }
        return ::connect(sa, (sockaddr*)&sai, sailen) == 0;
    }

    void send(void* sender, ZBuf* s) {
        if(!closed) {
            if(ext_queue == NULL && s != NULL) add(sender, s);
            //_onSend();
        } else {
            if( NULL != s) delete s;
        }
    }

    bool send(const char* buf, int size) {
        const char* old_buf = buf;
        int old_size = size;
        while(size > 0) {
            int count = _send(buf, size);
            if(count <= 0) {
                if(isBlock()) {
                    MsgQueue::zsleep(50);
                    continue;
                } else return false;
            }
            size -= count;
            buf += count;
        }
        onSend(old_buf, old_size);
        return true;
    }

    virtual void onRecv(ZBuf*) {}
    virtual void onSend(const char*, int) {}
    virtual void onConnect() {}
    virtual void onClose() {}

    char* udx_buf;
    unsigned int udx_length;
    int receive(char* buf, int buf_size) {
        if(type == CONN_UDP) {
            unsigned int copy_size = udx_length > (unsigned int)buf_size ? buf_size : udx_length;
            memmove(buf, udx_buf, copy_size);
            udx_buf += copy_size;
            udx_length -= copy_size;
            return copy_size > 0 ? copy_size : -1;
        } else return (int)::recv(sa, buf, buf_size, 0);
    }

    int _head_pos;
    msg_head _head;

    void _onRecv() {
        if(!closed) {
            int cb = 0;
            if(packet == NULL) {
                cb = receive((char*)&_head + _head_pos, sizeof(msg_head) - _head_pos);
                if(cb < 0 && isBlock()) return;
                if(cb > 0) {
                    _head_pos += cb;
                    if(_head_pos == sizeof(msg_head)) {
                        if(_head.magic != (unsigned int)0x04111420) {
                            remote_closed = true;
                            close();
                            //XLOG_FATAL("error magic close stream [sid:%s][ip:%s][port:%d][protocol:%s]", sid, IP(sai).c_str(), PORT(sai), conn_protocol_string());
                            //LOG4
                            return;
                        }
                        packet = new ZBuf(&_head);
                    }
                }
            } 
            if(packet != NULL) {
                cb = receive(packet->ptr, packet->get_remain_size());
                if(cb < 0 && isBlock()) return;
                if(cb > 0 && packet->fill(cb)) {
                    packet->start();
                    onRecv(packet);
                    delete packet;
                    packet = NULL;
                    _head_pos = 0;
                }
            }
            if(cb == 0) {
                remote_closed = true;  /* connenction closed by peer */
                close();
                //XLOG_INFO("recv fin close connection [sid:%s][ip:%s][port:%d][protocol:%s]", sid, IP(sai).c_str(), PORT(sai), conn_protocol_string());
            }
        }
    }

    char* _send_block;
    unsigned int _send_pos;
    int _send_size;

    bool hasData() {
        return MsgQueue::hasData(queue_pos) || (ext_queue != NULL && ext_queue->hasData(ext_pos));
    }

    void _onSend() {
        if (!__sync_bool_compare_and_swap(&sending, false, true)) return;
        while(!closed && hasData()) {
            if(_send_block == NULL) {
                BufPair* buf_pair = get(queue_pos);
                if(buf_pair == NULL && ext_queue != NULL) {
                    do {
                        buf_pair = ext_queue->get(ext_pos);
                    } while(buf_pair != NULL && buf_pair->sender == this);
                }
                if(buf_pair != NULL) {
                    ZBuf* buf = buf_pair->buf;
                    if(buf != NULL) {
                        _send_size = buf->get_size() + sizeof(msg_head);
                        _send_block = new char[_send_size];
                        _send_pos = 0;
                        memmove(_send_block, buf->buf, _send_size);
                    }
                }
            }
            if(_send_block != NULL) {
                int cb = _send(_send_block + _send_pos, _send_size);
                if(cb <= 0) {
                    if(cb == 0 || !isBlock()) {
                        if(cb == 0) {
                            //XLOG_FATAL("send failed remote closed [sid:%s][ip:%s][port:%d][protocol:%s]", sid, IP(sai).c_str(), PORT(sai), conn_protocol_string());
                            remote_closed = true;
                        }
                        close();
                    }
                    break;
                } else {
                    if(_send_size == cb) {
                        onSend(_send_block, _send_size + _send_pos);
                        delete[] _send_block;
                        _send_block = NULL;
                    } else {
                        _send_size -= cb;
                        _send_pos += cb;
                    }
                }
            }
        }
        sending = false;
    }

    const char* get_ip() {
        return IP(sai).c_str();
    }
    unsigned get_port() {
        return PORT(sai);
    }
};

template<typename T> class VNet: public IUdxTcpSink {
    IFastUdx* udx_obj;
    std::list<pthread_t> threads;
    pthread_t clean_id;
    pthread_mutex_t _net_mtx;
    public:
    bool bServer;
    void OnStreamRead(IUdxTcp * pTcp, BYTE* pData, int len) {
        if(pTcp->IsConnected()) {
            T* conn = (T*)(pTcp->GetUserData());
            if(checkAndDestroy(conn))
                return;

            if(conn != NULL) {
                conn->udx_buf = (char*)pData;
                conn->udx_length = len;
                while(conn->udx_length > 0 && !conn->closed && pTcp->IsConnected()) {
                    conn->_onRecv();
                }
            }
        }
    }

    void OnStreamBroken(IUdxTcp * pTcp) {
        T* conn = (T*)(pTcp->GetUserData());
        if(conn != NULL) {
            conn->close();
            //XLOG_INFO("close conn OnStreamBroken [sid:%s][ip:%s][port:%d][protocol:udp]",
                    //conn->sid, IP(conn->sai).c_str(), PORT(conn->sai));
        }
        if(checkAndDestroy(conn)) return;
    }

    void OnStreamNeedMoreData(IUdxTcp *pTcp, int) {
        if(pTcp->IsConnected()) {
            T* conn = (T*)(pTcp->GetUserData());
            if(checkAndDestroy(conn))
                return;
            if(conn != NULL)
                conn->_onSend();
        }
    }

    void OnStreamChancetoFillBuff(IUdxTcp *pTcp) {
        if(pTcp->IsConnected()) {
            T* conn = (T*)(pTcp->GetUserData());
            if(checkAndDestroy(conn))
                return;
            if(conn != NULL)
                conn->_onSend();
        }
    }

    void OnStreamConnect(IUdxTcp * pTcp, int erro) {
        if(bServer && erro == 0 && bUsable) {
            /* pTcp->SetTimeOut(10, 4, 20); */
            T* conn = new T;
            conn->type = CONN_UDP;
            conn->parent = (void*)this;
            conn->sai = *(sockaddr_in*)pTcp->GetRemoteAddr();
            conn->udx = pTcp;
            pTcp->SetUserData((INT64)conn);
            //XLOG_INFO("new connection [sid:%s][ip:%s][port:%d][protocol:udp]",
                    //conn->sid, IP(conn->sai).c_str(), PORT(conn->sai));
            conn->onConnect();
            {
                MyGuard guard(&_net_mtx);
                this->conns.push_back(std::make_pair(conn, DESTROY_CONN_DELAY_TIMES));
            }
        }
    }

    void OnStreamFinalRelease(IUdxTcp * pTcp){
        /*
        if(pTcp) {
            T* conn = (T*)(pTcp->GetUserData());
            XLOG_INFO("conn final release udx addr[0x%x] conn addr[0x%x]", 
                    (uint64_t)pTcp, (uint64_t)conn);
        } else {
            XLOG_INFO("conn final release udx is NULL");
        }
        */
    }

    /*
       void OnStreamFinalRelease(IUdxTcp * pTcp) {
       T* conn = (T*)(pTcp->GetUserData());
       if(conn != NULL) {
       if(conn->removed) delete conn;
       else conn->removed = true;
       }
       }
       */

    bool checkAndDestroy(T *conn) {
        bool ret = false;
        if(NULL != conn) {
            if(conn->closed && (conn->removed == false)) {
                if(conn->udx) { 
                    conn->udx->SetUserData((int64_t(NULL)));
                    conn->udx->Close();
                    conn->removed = true; 
                    ret = true;
                }
            } 
        } 
        return ret;
    }

    VNet() {
        bUsable = true;
        bStop = false;
        udx_obj = NULL;
        bServer = false;
        pthread_mutex_init(&_net_mtx, NULL);
        if(pthread_create(&clean_id, NULL, &clearConns, this) == 0) threads.push_back(clean_id);
    }

    ~VNet() {
        bStop = true;
        for(std::list<pthread_t>::iterator t = threads.begin(); t != threads.end(); ++t) pthread_join(*t, NULL);
    }

    static void handle_pipe(int sig) {}

    static void init() {
        struct sigaction action;
        action.sa_handler = handle_pipe;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGPIPE, &action, NULL);
    }

    bool bStop;
    bool bUsable;

    std::list<std::pair<T*, uint32_t> > conns;
    std::list<std::pair<T*, uint32_t> > closed_conns;

    static T* _accept(VNet<T>* net, int listener = -1) {
        struct sockaddr sai;
        socklen_t addrlen = sizeof(sockaddr_in);
        int sa = ::accept(listener, &sai, &addrlen);
        if(sa != -1) {
            if(net->bUsable) {
                T* conn = new T;
                conn->sa = sa;
                memmove(&conn->sai, &sai, addrlen);
                conn->type = CONN_TCP;
                conn->parent = (void*)net;
                conn->set_socket_opt();
                conn->onConnect();
                return conn;
            } else ::close(sa);
        }
        return NULL;
    }

    /*
     * @desc wait connections by epoll
     * @NOTICE do not use EPOLLET, or will be bug here!
     */
    static void epoll_loop(VNet<T>* net, int listener) {
        struct epoll_event ev, events[256];
        int epfd = epoll_create(1024);
        ev.data.fd = listener;
        /* ev.events = EPOLLIN | EPOLLET; */
        ev.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &ev);
        while(!net->bStop) {
            {
                MyGuard guard(&net->_net_mtx);
                for(typename std::list<std::pair<T*, uint32_t> >::iterator it = net->conns.begin(); it != net->conns.end(); ) {
                    T* conn = (T*)(it->first);
                    if (conn->type == CONN_UDP) {  /* ignore udp connection */
                        ++it;
                        continue;
                    }
                    if(!conn->closed) {
                        if(conn->hasData()){
                            ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLOUT; 
                            ev.data.fd = conn->sa;
                            ev.data.ptr = (void*)conn;
                            epoll_ctl(epfd, EPOLL_CTL_MOD, conn->sa, &ev);
                        } 
                        ++it;
                    } else {
                        net->closed_conns.push_back(std::make_pair(conn, DESTROY_CONN_DELAY_TIMES));
                        it = net->conns.erase(it);
                    }
                    /*
                     * TODO need test!
                    else {
                        if(conn->removed) {
                            delete conn;
                            conn = NULL;
                        }
                        it = net->conns.erase(it);
                    }
                    */
                }
            }
            int nfds = epoll_wait(epfd, events, 256, 50);
            for(int index = 0; index < nfds; index++) {
                if(events[index].data.fd == listener) {  /* new connection */
                    T* conn = _accept(net, listener);
                    if(conn != NULL) {
                        ev.data.fd = conn->sa;
                        ev.data.ptr = (void*)conn;
                        ev.events = EPOLLIN | EPOLLRDHUP;
                        epoll_ctl(epfd, EPOLL_CTL_ADD, conn->sa, &ev);
                        {
                            MyGuard guard(&net->_net_mtx);
                            net->conns.push_back(std::make_pair(conn, DESTROY_CONN_DELAY_TIMES));
                        }
                        //XLOG_INFO("new connection [sid:%s][ip:%s][port:%d][protocol:tcp]", conn->sid, IP(conn->sai).c_str(), PORT(conn->sai));
                    }
                } else if(events[index].data.fd != -1) {
                    T* conn = (T*)events[index].data.ptr;
                    if(events[index].events & EPOLLIN) conn->_onRecv();
                    if(events[index].events & EPOLLOUT) conn->_onSend();
                    if(events[index].events & EPOLLRDHUP) {
                        conn->close();
                        conn->remote_closed = true;
                        //XLOG_INFO("remote close connection [sid:%s][ip:%s][port:%d][protocol:tcp]", conn->sid, IP(conn->sai).c_str(), PORT(conn->sai));
                    } else if(events[index].events & EPOLLERR || events[index].events & EPOLLHUP) {
                        conn->close();
                        //XLOG_INFO("close conn in epoll_loop [sid:%s][ip:%s][port:%d][protocol:tcp]", conn->sid, IP(conn->sai).c_str(), PORT(conn->sai));
                    }
                    if(conn->closed) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, conn->sa, NULL);
                        conn->removed = true;
                    }
                    else {
                        ev.events = EPOLLIN | EPOLLRDHUP; 
                        ev.data.fd = conn->sa;
                        ev.data.ptr = (void*)conn;
                        epoll_ctl(epfd, EPOLL_CTL_MOD, conn->sa, &ev);
                    }
                }
            }
        }
        ::close(epfd);
    }

    /**
     * @desc wait connections by select
     */
    static bool selectConns(VNet<T>* net, int listener = -1) {
        int fdmax = listener;
        fd_set read_fds;
        fd_set write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        if(listener != -1) FD_SET(listener, &read_fds);
        {
            MyGuard guard(&net->_net_mtx);
            for(typename std::list<std::pair<T*,uint32_t> >::iterator it = net->conns.begin(); it != net->conns.end();) {
                T* conn = (T*)(it->first);
                if (conn->type == CONN_UDP) {  /* ignore udp connection */
                    ++it;
                    continue;
                }
                if(!conn->closed) {
                    FD_SET(conn->sa, &read_fds);
                    if(conn->hasData()) FD_SET(conn->sa, &write_fds);
                    if(conn->sa > fdmax) fdmax = conn->sa;
                    ++it;
                } else {
                    net->closed_conns.push_back(std::make_pair(conn, DESTROY_CONN_DELAY_TIMES));
                    it = net->conns.erase(it);
                }
                /* else {
                   if(conn->removed) delete conn;
                   it = net->conns.erase(it);
                   }
                */
        }
        }
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 50 * 1000;
        int nfds = select(fdmax + 1, &read_fds, &write_fds, NULL, &timeout);
        if(nfds == -1) return (listener != -1 || net->conns.size() > 0);
        if(listener != -1 && FD_ISSET(listener, &read_fds)) {
            T* conn = _accept(net, listener);
            if(conn != NULL) net->conns.push_back(std::make_pair(conn, DESTROY_CONN_DELAY_TIMES));
        }
        if(net->conns.size() > 0) {
            for(typename std::list<std::pair<T*, uint32_t> >::iterator c = net->conns.begin(); c != net->conns.end(); c++) {
                T* conn = (T*)(c->first);
                if (conn->type == CONN_UDP) {
                    continue;
                }
                if(FD_ISSET(conn->sa, &read_fds)) conn->_onRecv();
                if(FD_ISSET(conn->sa, &write_fds)) conn->_onSend();
            }
        }
        return listener != -1 || net->conns.size() > 0;
    }

#ifdef WIN32
#define ThreadType static void
#else
#define ThreadType static void*
#endif

    sockaddr_in listen_addr;
    ThreadType processUDP(void* param) {
        return NULL;
    }

    ThreadType processTCP(void* param) {
        VNet<T>* net = (VNet<T>*)param;
        int _s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(_s != -1) {
            int yes = 1;
            if(setsockopt(_s, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) != -1) {
                int ret = ::bind(_s, (struct sockaddr *)&net->listen_addr, sizeof(net->listen_addr));
                if (ret == -1) {
                    //XLOG_FATAL("%s", "bind error type[tcp]");
                } else if(listen(_s, 10) == -1) {
                    //XLOG_FATAL("%s", "listen error type[tcp]");
                } else {
                    epoll_loop(net, _s);
                    /* use select  --  while(!net->bStop && selectConns(net, _s)); */
                }
            }
            ::close(_s);
        }
        return NULL;
    }

    ThreadType processConnect(void* param) {
        VNet<T>* net = (VNet<T>*)param;
        while(!net->bStop && selectConns(net));
        return NULL;
    }

    ThreadType clearConns(void* param) {
        VNet<T>* net = (VNet<T>*)param;
        while(!net->bStop) {
            {
                MyGuard guard(&net->_net_mtx);
                for(typename std::list<std::pair<T*,uint32_t> >::iterator it = net->conns.begin(); it != net->conns.end();) {
                    T* conn = (T*)(it->first);
                    if(conn->removed && (0 == --(it->second))) {
                        //XLOG_INFO("free conn [sid:%s][ip:%s][port:%d][protocol:%s]", 
                                //conn->sid, conn->get_ip(), conn->get_port(), conn->conn_protocol_string());
                        it = net->conns.erase(it);
                        conn->atClearConn();
                        delete conn;
                        conn = NULL;
                    } else ++it;
                }
                for(typename std::list<std::pair<T*,uint32_t> >::iterator it = net->closed_conns.begin(); it != net->closed_conns.end();) {
                    T* conn = (T*)(it->first);
                    if(conn->removed && (0 == --(it->second))) {
                        //XLOG_INFO("free conn in closed_conns [sid:%s][ip:%s][port:%d][protocol:%s]", 
                                //conn->sid, conn->get_ip(), conn->get_port(), conn->conn_protocol_string());
                        it = net->closed_conns.erase(it);
                        conn->atClearConn();
                        delete conn;
                        conn = NULL;
                    } else ++it;
                }
            }
            MsgQueue::zsleep(1000);
        }
        return NULL;
    }

    T* raw_connect(sockaddr_in addr, ConnType type = CONN_UDP) {
        T* conn = new T;
        conn->type = type;
        conn->sai = addr;
        if(type == CONN_UDP) {
            if(udx_obj == NULL) {
                udx_obj = CreateFastUdx();
                udx_obj->SetSink(this);
                udx_obj->Create();
                udx_obj->SetConnectTimeOut(1);
            }
            char buf[64] = {'\0'}; 
            inet_ntop(AF_INET, &addr.sin_addr.s_addr, buf, sizeof(buf));
            conn->udx = udx_obj->Connect(buf, ntohs(addr.sin_port), 1);
            //conn->udx = udx_obj->Connect(inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), 1);
        } else conn->sa = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(conn->connect()) return conn;
        else {
            conn->close();
            //XLOG_FATAL("close conn raw_connect error [sid:%s][ip:%s][port:%d][protocol:%s]",
                    //conn->sid, IP(conn->sai).c_str(), PORT(conn->sai), conn->conn_protocol_string());
            delete conn;
            return NULL;
        }
    }

    void _post_connect(T* conn) {
        conn->set_socket_opt();
        conns.push_back(make_pair(conn, DESTROY_CONN_DELAY_TIMES));
        //XLOG_INFO("add conn [sid:%s][ip:%s][port:%d][protocol:tcp]", conn->sid, conn->get_ip(), conn->get_port());
        pthread_t ntid;
        if(pthread_create(&ntid, NULL, &processConnect, this) == 0) threads.push_back(ntid);
    }

    /*
     *autoFreeConn: 1 auto free; 0 no auto free, must free by user
     * */
    T* connect(sockaddr_in addr, ConnType type = CONN_UDP, int autoFreeConn = 1) {
        T* conn = raw_connect(addr, type);
        if(conn != NULL) {
            conn->parent = (void*)this;
            if(type == CONN_UDP) {
                MyGuard guard(&_net_mtx);
                if(autoFreeConn == 1) {
                    conns.push_back(make_pair(conn, DESTROY_CONN_DELAY_TIMES));
                }
                //XLOG_INFO("add conn [sid:%s][ip:%s][port:%d][protocol:udp]", conn->sid, conn->get_ip(), conn->get_port());
                return conn;
            }
            /* UDP never reach here */
            _post_connect(conn);
        }
        return conn;
    }

    T* connect(char* ip, unsigned int port, ConnType type = CONN_UDP, int autoFreeConn = 1) {
        std::string str_ip(ip);
        sockaddr_in addr = VNetCon::getAddr(str_ip, port);
        return connect(addr, type, autoFreeConn);
    }

    bool reconnect(T* conn) {
        if(conn->closed) {
            if(conn->type == CONN_UDP) {
                char buf[64] = {'\0'}; 
                inet_ntop(AF_INET, &(conn->sai.sin_addr.s_addr), buf, sizeof(buf));
                conn->udx = udx_obj->Connect(buf, ntohs(conn->sai.sin_port), 1);
                if(conn->udx != NULL && conn->connect()) {
                    conn->closed = conn->remote_closed = false;
                    conn->onConnect();
                    //XLOG_INFO("reconnect success [sid:%s][ip:%s][port:%d][protocol:udp]", conn->sid, conn->get_ip(), conn->get_port());
                    return true;
                } else return false;
            } else {
                conn->sa = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if(conn->connect()) {
                    conn->closed = conn->remote_closed = false;
                    conn->onConnect();
                    _post_connect(conn);
                    //XLOG_INFO("reconnect success [sid:%s][ip:%s][port:%d][protocol:tcp]", conn->sid, conn->get_ip(), conn->get_port());
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * @param [addr] ip/port of this relay server
     */
    bool startServer(sockaddr_in addr, ConnType type) {
        this->listen_addr = addr;
        if(type == CONN_UDP) {
            if(udx_obj == NULL) {
                udx_obj = CreateFastUdx();
                udx_obj->SetSink(this);

                char buf[64] = {'\0'}; 
                inet_ntop(AF_INET, &addr.sin_addr.s_addr, buf, sizeof(buf));

                udx_obj->Create(buf, ntohs(addr.sin_port));
                //udx_obj->Create(inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                bServer = true;
            }
            return true;
        }
        pthread_t ntid;
        if(type == CONN_UDP) {if(pthread_create(&ntid, NULL, &processUDP, this) == 0) threads.push_back(ntid);}
        else if(pthread_create(&ntid, NULL, &processTCP, this) == 0) threads.push_back(ntid);
        return true;
    }
};

#endif
