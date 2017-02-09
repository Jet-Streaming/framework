#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>
#include <sstream>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <list>

#pragma pack(push, 2)
struct msg_head {
	unsigned int magic;																	//magic is 0x20141104(BE) 
	unsigned short type;
	unsigned int size;
	unsigned int get_size() {
		return ntohl(size);
	}
	void set_size(unsigned int _size) {
		size = htonl(_size);
	}
	unsigned short get_type() {
		return ntohs(type);
	}
    unsigned int get_magic() {
		return (unsigned int)ntohl(magic);
	}
	void set_type(unsigned short _type) {
		type = htons(_type);
	}
};

struct tlv_header {																		//the old tlv(type length value) struct
	unsigned short type;
	unsigned short length;
	unsigned short get_type() {
		return ntohs(type);
	}
	unsigned short get_length() {
		return ntohs(length);
	}
	void set_type(unsigned short _type) {
		type = htons(_type);
	}
	void set_length(unsigned short _length) {
		length = htons(_length);
	}
};
#pragma pack(pop)

#define MIN_SEND_BUF	1024
class ZBuf {
public:
	char* buf;																			//data buffer with a msg_head head
	char* ptr;																			//write or read pointer

	virtual ~ZBuf() {
		if(buf != NULL) delete[] buf;
	}

	void start() {
		ptr = buf + sizeof(msg_head);
	}

	unsigned int get_size() {
		if(buf != NULL) return ((msg_head*)buf)->get_size();
		else return 0;
	}

	unsigned short get_type() {
		if(buf != NULL) return ((msg_head*)buf)->get_type();
		else return 0;
	}

	unsigned int get_data_size() {
		if(buf != NULL) return (unsigned int)(ptr - buf) - sizeof(msg_head);
		else return 0;
	}

	unsigned int get_remain_size() {
		if(buf != NULL) return ((msg_head*)buf)->get_size() - get_data_size();
		else return 0;
	}

	unsigned int get_magic() {
		if(buf != NULL) return ((msg_head*)buf)->get_magic();
		else return 0;
	}


	ZBuf(unsigned short type) {
		buf = new char[MIN_SEND_BUF];
		((msg_head*)buf)->set_size(MIN_SEND_BUF - sizeof(msg_head));
		((msg_head*)buf)->set_type(type);
		((msg_head*)buf)->magic = 0x04111420;
		ptr = buf + sizeof(msg_head);
	}

	ZBuf(msg_head* head) {
		buf = new char[head->get_size() + sizeof(msg_head)];
		memmove(buf, head, sizeof(msg_head));
		ptr = buf + sizeof(msg_head);
	}

	ZBuf(ZBuf* _buf) {
		unsigned int _size = ((msg_head*)_buf->buf)->get_size() + sizeof(msg_head);
		buf = new char[_size];
		memmove(buf, _buf->buf, _size);
		ptr = buf + _size;
	}

	tlv_header* getTLV() {
		unsigned int data_size = get_size() - (ptr - buf - sizeof(msg_head));
		if(data_size > sizeof(tlv_header)) {
			tlv_header* tlv = (tlv_header*)ptr;
			if(tlv->get_length() + sizeof(tlv_header) <= data_size) {
				ptr += tlv->get_length() + sizeof(tlv_header);
				return tlv;
			}
		}
		return NULL;
	}

	bool write(const char* data, unsigned int count) {
		unsigned int data_size = get_data_size();
		unsigned int free_size = get_remain_size();
		if(free_size < count) {						//no enough buffer
			free_size = get_size() * 2 > data_size + count ? get_size() * 2 : data_size + count;
			char* new_buf = new char[free_size + sizeof(msg_head)];
			if(new_buf == NULL) return false;
			memmove(new_buf, buf, data_size + sizeof(msg_head));
			delete[] buf;
			buf = new_buf;
			((msg_head*)buf)->set_size(free_size);
			ptr = new_buf + data_size + sizeof(msg_head);
		}
		memmove(ptr, data, count);
		ptr += count;
		return true;
	}

	void finish() {
		((msg_head*)buf)->set_size((unsigned int)(ptr - buf) - sizeof(msg_head));
		ptr = buf;
	}

	bool fill(unsigned int fill_size) {
		ptr += fill_size;
		if(get_data_size() == ((msg_head*)buf)->get_size()) return true;
		return false;
	}
};

#ifndef WIN32
#include <pthread.h>
#include <semaphore.h>
#else 
typedef  CRITICAL_SECTION pthread_mutex_t;
#endif

class MyGuard {
	pthread_mutex_t* mutex;
public:
	MyGuard(pthread_mutex_t* _mutex) : mutex(_mutex) {
#ifdef WIN32
		EnterCriticalSection(mutex);
#else
		pthread_mutex_lock(mutex);
#endif
	}
	~MyGuard() {
#ifdef WIN32
		LeaveCriticalSection(mutex);
#else
		pthread_mutex_unlock(mutex);
#endif
	}
};

using namespace std;

struct BufPair {
    void* sender;
    ZBuf* buf;
    BufPair() {
        sender = NULL;
        buf = NULL;
    }
};

typedef int(*DropFunc)(BufPair*, int, void* context);

class MsgQueue {
public:
    inline MsgQueue(unsigned int _max = 512, bool _multi=false);
    inline virtual ~MsgQueue();

    inline bool hasData(unsigned int pos);
    inline void setDrop(DropFunc drop);
    inline void add(void* sender, ZBuf* s);
    inline BufPair* get(unsigned int& offset);
    // 建议使用下面的函数。
    inline BufPair* get(unsigned int& offset, BufPair& pBuf);
    DropFunc drop_func;

    inline static void zsleep(unsigned int mseconds);

public:
    int             max_number;
    unsigned int    buf_start;
protected:
    inline BufPair* multiGet(unsigned int& offset);
    inline BufPair* singleGet(unsigned int& offset);

    inline void clearDorp();
private:
    pthread_mutex_t _buf_mtx;
    BufPair*        send_bufs;
    int             buf_number;
    unsigned long   mAllCount;

    bool     isMulti;

    std::list<ZBuf*>  mDropList;
};

MsgQueue::MsgQueue(unsigned int _max, bool _multi): max_number(_max), isMulti(_multi) {
	buf_start = 0;
	send_bufs = new BufPair[max_number];
	buf_number = 0;
	mAllCount = 0;
	drop_func = NULL;
	pthread_mutex_init(&_buf_mtx, NULL);
}

MsgQueue::~MsgQueue() {
	clearDorp();
	for(int index = 0; index < buf_number; index++) {
		delete send_bufs[buf_start].buf;
		send_bufs[buf_start].buf = NULL;
		buf_start = (buf_start+1)%max_number;
	}
	delete[] send_bufs;
    pthread_mutex_destroy(&_buf_mtx);
}

void MsgQueue::clearDorp(){
	if( mDropList.size() > 0 ){
		for( std::list<ZBuf*>::iterator _it = mDropList.begin(); _it != mDropList.end(); ++_it){
			delete (*_it);
		}

		mDropList.clear();
	}
}

bool MsgQueue::hasData(unsigned int pos) {
	MyGuard guard(&_buf_mtx);
	return (int)(mAllCount - pos) > 0;
}

void MsgQueue::zsleep(unsigned int mseconds) {
	usleep(mseconds * 1000);
}

void MsgQueue::setDrop(DropFunc drop) {
	drop_func = drop;
}

void MsgQueue::add(void* sender, ZBuf* s) {
	MyGuard guard(&_buf_mtx);
	if(buf_number >= (int)max_number) {
		int erase = max_number >> 2;		//drop 25% packets avoid 
		for(int index = 0; index < erase; index++) {
			mDropList.push_back(send_bufs[buf_start].buf);
			send_bufs[buf_start].buf = NULL;
			buf_start = (buf_start + 1) % max_number;
		}
		buf_number -= erase;
	}
	s->finish();
	int pos = (buf_start + buf_number) % max_number;
	send_bufs[pos].buf = s;
	send_bufs[pos].sender = sender;
	buf_number++;
	mAllCount++;
}

BufPair* MsgQueue::multiGet(unsigned int& offset){
	BufPair* tmp = singleGet(offset);
	BufPair* res = NULL;

	if (NULL != tmp) {
		res = new BufPair;
		res->sender = tmp->sender;
		ZBuf* _buf = new ZBuf(tmp->buf);
		res->buf = _buf;
	}
	return res;
}

BufPair* MsgQueue::singleGet(unsigned int& offset){
	MyGuard guard(&_buf_mtx);
	clearDorp();
	if(buf_number == 0 || offset >= mAllCount) return NULL;

	int pos = (offset % max_number);
    if ((int)(mAllCount - offset) > max_number) offset = mAllCount - buf_number - 1;
    if (offset < mAllCount - buf_number){
		offset = mAllCount - buf_number;
		pos = buf_start;
		if(drop_func != NULL) {
			int stp = drop_func(send_bufs, buf_number, this);
			offset += stp;
			pos = ((buf_start + stp)%max_number);
		}
	}
	offset++;
	return &send_bufs[pos];
}

BufPair* MsgQueue::get(unsigned int& offset) {
	if( isMulti ) return multiGet(offset);
	return singleGet(offset);
}

BufPair* MsgQueue::get(unsigned int& offset, BufPair& pBuf) {
	BufPair* tmp = singleGet(offset);

    if( NULL == tmp) return NULL;
    pBuf.sender = tmp->sender;
    pBuf.buf = tmp->buf;
    return &pBuf;
}

#define NODE_UP_READY		1
#define NODE_DOWN_READY		2

#define MSG_TYPE_RELAY_MANAGER		100			//message's type from relay to manager
#define MSG_TYPE_MANAGER_RELAY		101			//message's type from relay to manager
#define MSG_TYPE_RELAY_RELAY		102			//message's type from relay to relay
#define MSG_TYPE_MANAGER_INTERFACE	103
#define MSG_TYPE_PEER_MSG			104

#define MSG_LOGIN				0
#define MSG_START_STREAM		1
#define MSG_STOP_STREAM			2
#define MSG_UPDATE_STREAM		3
#define MSG_PUSH_STREAM			4
#define MSG_RELAY_STREAM		5
#define MSG_GET_STREAM			6
#define MSG_PULL_STREAM			7
#define MSG_UPDATE_STREAM_V2    8 /**< not used now */
#define MSG_UPDATE_RELAY        9

#define MSG_GET_UP_NODE			10
#define MSG_GET_DOWN_NODE		11
#define MSG_GET_STREAM_UP_NODE	12

#define MSG_GET_NODE_LIST		20
#define MSG_GET_STREAM_LIST		21

#define MSG_UPDATE_STATUS		30

enum {
	REQUEST_TYPE_IPC_STREAM = 1,
	IPC_STREAM_GRAPIC_I,
	IPC_STREAM_GRAPIC_P,
	IPC_STREAM_VOICE,
	TYPE_RESPOND_ERRCODE,
	REQUEST_TYPE_APP_STREAM,
	REQUEST_TYPE_MEASURE,
	MSG_TYPE_FRAME_B = 0x0008,                                                             
	MSG_TYPE_CONTROL = 0x0009,
	IPC_STREAM_VOICE_AAC = 10,
	MSG_TYPE_BROADCAST,
};



//TLV TYPE
enum {
	TYPE_SN_TXT = 1,
	TYPE_SN_ENCRYPT,
	TYPE_ERRCODE,
	TYPE_NEED_RECONNECT,
	TYPE_CLUSTER_ID = 5,
	TYPE_IS_CLOSE_STREAM,
	TYPE_STREAM_ORI,
	TYPE_STREAM_TYPE,
	TYPE_STREAM_VERSION,
	TYPE_STREAM_KEY = 10,
	TYPE_APP_NAME,
    TYPE_KEEP_ALIVE,
};

#endif

