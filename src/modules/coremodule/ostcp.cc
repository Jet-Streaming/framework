#include "ostcp.h"
//#include <jet/buffer.h>
#include <jet/config.h>
#include <jet/stream.h>
//#include <jet/pad.h>

namespace jet {
namespace coremodule {

ostcp::ostcp()
    : data_pad("data", this, on_push)
{
    int rc;

	wreq.data = nullptr;
    rc = data_pad.set_cap(pad::direction::DIN, "stream");
    assert(EOK == rc);
    rc = data_pad.set_cap(pad::direction::DOUT, "stream");
    assert(EOK == rc);

    rc = uv_tcp_init(uv_default_loop(), &uvtcp);
    assert(0 == rc);
}

ostcp::~ostcp()
{
    uv_close((uv_handle_t *)&uvtcp, nullptr);
}

int 
ostcp::bind(const char *ip, uint16_t port)
{
    int rc;
    struct sockaddr_in saddr;

    rc = uv_ip4_addr(ip, port, &saddr);
    if (rc) {
        return rc;
    }
    rc = uv_tcp_bind(&uvtcp, (struct sockaddr *)&saddr, 0);
    return rc;
}

void
ostcp::on_listen(uv_stream_t* server, int status)
{
    ostcp *self;
    listen_cb cb;

    self = container_of((uv_tcp_t *)server, ostcp, uvtcp);
    cb = (listen_cb)self->uvtcp.data;
    cb(self, status);
}

int 
ostcp::listen(int backlog, listen_cb cb)
{
    int rc;

    rc = uv_listen((uv_stream_t *)&uvtcp, backlog, on_listen);
    if (rc) {
        return rc;
    }
    uvtcp.data = cb;
    return rc;
}

// TODO 参数3的类型改为jmbuf_t*
//static jmbuf_t *bbbb;
void
ostcp::on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    ostcp *self;
    packet *pkt;
    int rc;

    self = container_of((uv_tcp_t *)handle, ostcp, uvtcp);
    buf->len = 0;
    if (self->pushout.pkt) {
        // not acked
        return;
    }

    // 该模块只是从kernel buffer获取数据，如果是utcp，则是rte_eth_rx_burst（底层驱动才知道的MTU）
    //mbuf = jmbuf_new(NULL, &tcp->pad); // TODO：可以实时自动调整，以节省内存
	//pkt = new packet();
	pkt = new stream();
    if (rc = pkt->alloc()) {
        self->abort();
        return;
    }
    assert(!self->pushout.pkt);
    self->pushout.pkt = pkt;

    buf->base = (char *)pkt->data() + pkt->len();
    buf->len = pkt->tailroom();
}

void
ostcp::on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    packet *pkt;
    ostcp *self;
    int rc;
    char *pkt_data;
    
    self = container_of((uv_tcp_t *)stream, ostcp, uvtcp);
    if (-ENOBUFS == nread) {
        rc = uv_read_stop(stream);
        assert(EOK == rc);
        return;
    } else if (nread < 0) {
        //rc = nread;
        self->abort();
        printf("ostcp error: %d\n", nread);
        return;
    }

    // dump memory
#if 0
    puts("{{{{{");
    for (rc = 0; rc != nread; ++rc) {
        printf("%02x ", ((uint8_t *)(buf->base))[rc]);
        if (0 == rc) continue;
        int i = rc + 1;
        if (0 == (i % 8)) {
            putchar('  ');
        }
        if (0 == (i % 16)) {
            putchar('\n');
        }
    }
    puts("\n}}}}}");
#endif

    pkt = self->pushout.pkt;
    self->pushout.pkt = nullptr;
    assert(pkt);

    //push出去后就不用管了，此处不负责释放
#if 1
    assert(nread > 0);
    pkt_data = pkt->append(nread);
    assert(pkt_data);
    //rc = jpad_push(&tcp->pad, tcp->out_push, mbuf);
    //rc = self->pushout
    rc = self->data_pad.push(&self->pushout, pkt);
    if (rc) {
        self->abort();
        return;
    }
#else
    int i;
    mbuf->buf_len = 1;
    for (i = 0; i != nread; ++i) {
        rc = jpad_push(&tcp->pad, mbuf, dummy_cb);
        mbuf->data++;
    }
#endif
}

int 
ostcp::accept(tcp *client)
{
    ostcp *c;
    int rc;

    c = static_cast<ostcp *>(client);
    rc = uv_accept((uv_stream_t *)&uvtcp, (uv_stream_t *)&c->uvtcp);
    if (rc) {
        return rc;
    }
    rc = uv_read_start((uv_stream_t *)&c->uvtcp, on_alloc, on_read);
    return rc;
}

void 
ostcp::on_connect(uv_connect_t* req, int status)
{
    ostcp *self;
    connect_cb cb;
    int rc;

    self = container_of(req, ostcp, connreq);
    cb = (connect_cb)self->uvtcp.data;
    cb(self, status);
    rc = uv_read_start((uv_stream_t *)&self->uvtcp, on_alloc, on_read);
}

int 
ostcp::connect(const char *ip, uint16_t port, connect_cb cb)
{
    int rc;
    //ostcp *self;
    struct sockaddr_in saddr;

    rc = uv_ip4_addr(ip, port, &saddr);
    if (rc) {
        return rc;
    }

    rc = uv_tcp_connect(&connreq, &uvtcp, (struct sockaddr *)&saddr, on_connect);
    if (rc) {
        return rc;
    }
    uvtcp.data = cb;
    return rc;
}

//static void
//push_cb(jpad_push_t *push)
//{
//    jtcp_t *tcp;
//
//    tcp = container_of(push->sender, jtcp_t, pad);
//    assert(tcp->out_push == push);
//    tcp->out_push = NULL;
//
//    //jbuffer_free(mbuf);
//}
//
//static void
//dummy_cb(jmbuf_t *mbuf, int status)
//{
//}
//
//
//
////typedef struct {
////    uv_write_t wreq;
////    jmbuf_t *mbuf;
////} jtcp_write_t;
//

int
ostcp::on_push(pad *pad, pad::push_req *req, pad::push_type type)
{
    int rc;
    ostcp *self;

    self = container_of(pad, ostcp, data_pad);
    switch (type)
    {
    case pad::PACKET:
        rc = self->on_packet(req);
        break;
    case pad::ACK:
        rc = self->on_ack(req);
        break;
    case pad::CANCEL:
        rc = self->on_cancel(req);
        break;
    case pad::UNLINK:
        rc = self->on_unlink(req);
        break;
    default:
        rc = -EINVAL;
        break;
    }

    //tcp->in_push = push;

    return rc;
}

void
ostcp::write_cb(uv_write_t* wreq, int status)
{
    ostcp *self;
    //jtcp_t *tcp;
    ////jmbuf_t *mbuf;
    int rc;
    pad::push_req *req;

    self = container_of(wreq, ostcp, wreq);
    req = static_cast<pad::push_req *>(wreq->data);
    if (!req) {
        return;
    }
    wreq->data = nullptr;
    if (EOK == req->status) {
        req->status = status;
    }
    self->data_pad.ack_push(req);
}

int 
ostcp::on_packet(pad::push_req *req)
{
    uv_buf_t uvb;

    if (wreq.data) {
        return -EAGAIN;
    }

    uvb.base = (char *)req->pkt->data();
    uvb.len = req->pkt->len();
    ////wreq->data = mbuf;
    wreq.data = req;
    return uv_write(&wreq, (uv_stream_t *)&uvtcp, &uvb, 1, write_cb);
}

int 
ostcp::on_ack(pad::push_req *req)
{
    //assert(pushout.pending);
    //pushout.pending = 0;
    return EOK;
}

int 
ostcp::on_cancel(pad::push_req *req)
{
#if UV_VERSION_MAJOR > 1
    // TODO CancelIoEx(); (within libuv 2.0)
    assert(false);
#endif
    if (wreq.data) {
        assert(req == wreq.data);
        req->status = -ECANCELED;
    }
    return EOK;
}

int 
ostcp::on_unlink(pad::push_req *req)
{
    wreq.data = nullptr;
    return EOK;
}


} // coremodule
} // jet


//static int
//inst_init(jostcp_t *self, va_list args)
//{
//    int rc;
//    jtcp_t *tcp;
//    jpad_t *pad;
//
//    tcp = (jtcp_t *)self;
//    pad = &tcp->pad;
//    pad->on_push = on_push;
//
//#if 0
//    // 模拟dpdk
//    pad->seginfo.maxlen = 6 + 6 + 2 + 1500;
//    pad->seginfo.headroom = /* MAC */6 + 6 + 2 + /* IP */20 + /* TCP */20;
//#else
//    pad->seginfo.maxlen = jconf_data()->def_seginfo.maxlen;
//    pad->seginfo.headroom = 3;
//#endif
//    pad->seginfo.minlen = 0;
//    pad->seginfo.valid = 1;
//
//    self->wreq.data = NULL;
//    rc = uv_tcp_init(uv_default_loop(), &self->tcp);
//
//    return rc;
//}
//
//static void
//vtbl_init(jostcp_class_t *self)
//{
//    jobject_class_t *obj;
//    jelement_class_t *elm;
//    jtcp_class_t *tcp;
//
//    obj = (jobject_class_t *)self;
//
//    //props[0].method.type = "int (*)(jtcp_t *self, int backlog)";
//    //props[0].method.entry = jtcp_listen;
//    //obj->properties = props;
//    //obj->nproperties = ARRAY_SIZE(props);
//
//    elm = (jelement_class_t *)self;
//    //elm->inst_size = sizeof(jostcp_t);
//    elm->name = "ostcp";
//
//    tcp = (jtcp_class_t *)self;
//    tcp->bind = _bind;
//    tcp->listen = _listen;
//    tcp->accept = _accept;
//    tcp->connect = _connect;
//
//}
