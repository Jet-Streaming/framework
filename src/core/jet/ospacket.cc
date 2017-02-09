#include "ospacket.h"

namespace jet {

static uint32_t
def_headroom = 24;
static uint32_t
def_tailroom = 0;
static uint32_t
def_bodyroom = 2048 - def_headroom - def_tailroom;


//rt_class 
//packet::classpacket = {
//    "packet",
//    sizeof(packet),
//};
//
//static const rt_classinit
//classinitpacket(&packet::classpacket);

JET_IMPLEMENT_ROOT_RTCLASS(packet);

bool 
packet::is_kind_of(const rt_class *klass) const
{
    return false;
}


////////////////////////////////////////

packet::packet()
    : object(new packet_private)
	, _buf_addr(nullptr)
    , _buf_len(0)
    , _data_off(0)
    , _data_len(0)
    , next(nullptr)
{}

packet::~packet()
{
    //::free(_buf_addr);
}

void *
packet::data() 
{
    return (char *)_buf_addr + _data_off;
}

uint32_t 
packet::len() 
{
    return _data_len;
}


void *
packet::buf_addr() 
{
    return _buf_addr;
}

uint32_t 
packet::buf_len() 
{
    return _buf_len;
}

int 
packet::alloc(uint32_t s) 
{
    assert(nullptr == _buf_addr);

    if (0 == s) {
        s = def_bodyroom;
    }
    s += def_headroom;

    _buf_addr = malloc(s);
    if (!_buf_addr) {
        return -ENOMEM;
    }
    _buf_len = s;
    _data_off = def_headroom;

    return EOK;
}

void
packet::reset()
{
    next = nullptr;
    _data_len = 0;
}

packet *
packet::lastseg()
{
    packet *p = this;
    while (p->next) {
        p = p->next;
    }
    return p;
}

int 
packet::chain(packet *tail)
{
    packet *p;

    p = lastseg();
    p->next = tail;
    return EOK;
}

uint32_t
packet::headroom()
{
    return _data_off;
}

uint32_t 
packet::tailroom()
{
    return _buf_len - _data_off - _data_len;
}

char *
packet::prepend(uint32_t len)
{
    if (len > headroom())
        return nullptr;

    _data_off -= len;
    _data_len += len;
    //m->pkt_len = (m->pkt_len + len);

    return (char *)_buf_addr + _data_off;
}

char *
packet::append(uint32_t len)
{
    void *tail;

    if (len > tailroom())
        return nullptr;

    tail = (char *)_buf_addr + _data_off + _data_len;
    _data_len += len;
    //m->pkt_len = (m->pkt_len + len);
    return (char*)tail;
}

char *
packet::adj(uint32_t len)
{
    if (len > _data_len) {
        return nullptr;
    }

    _data_len -= len;
    _data_off += len;
    return (char *)_buf_addr + _data_off;
}


//packet *
//packet::clone() 
int 
packet::clone(packet *dst)
{
    memcpy(dst, this, sizeof(packet));
    dst->next = nullptr;
    return EOK;
}

int 
packet::copy(const void *src, uint32_t s)
{
    void *dst;

    dst = append(s);
    if (!dst) {
        return -ENOMEM;
    }
    memcpy(dst, src, s);
    return EOK;
}


} // jet