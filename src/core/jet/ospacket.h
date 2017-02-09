#pragma once
#include "runtime_class.h"


namespace jet {

// 定义framework、module需要用的API
// tcp.ustack等自行实现rte_mbuf(DPDK)和packet的互转

class packet_private;
class J_EXTERN packet : public object
{
	J_DECLARE_PRIVATE(packet);
public:
    JET_DECLARE_RTCLASS(packet);
    bool is_kind_of(const rt_class *klass) const;

	packet();
	~packet();
    packet &operator =(const packet &) = delete;
    packet(const packet &) = delete;

    // current packet data pointer
    void *data();
    // sum of data lengths over all packet segments.
    uint32_t len();

    // start address of the first segment.
    void *buf_addr();
    // sum of buffer lengths over all packet segments.
    uint32_t buf_len();

    uint32_t headroom();
    uint32_t tailroom();
    char *prepend(uint32_t len);
    char *append(uint32_t len);
    char *adj(uint32_t len);
    //char *trim(uint32_t len);

    void reset();

    // ref

    // nb_segs
    //virtual uint32_t;

    //virtual void *user_data() = 0;

    int alloc(uint32_t s = 0);
    //packet *clone();
    int clone(packet *dst);
    packet *lastseg();
    int chain(packet *tail);

    // non DPDK
    int copy(const void *src, uint32_t s);

private:
    void *_buf_addr;
    uint32_t _buf_len;
    uint32_t _data_off;
    uint32_t _data_len;
    packet *next;
};

class J_EXTERN packet_private : public object_private
{
	J_DECLARE_PUBLIC(packet);

public:
	packet_private() {}
	virtual ~packet_private() {}
};


} // jet