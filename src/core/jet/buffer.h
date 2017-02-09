/*
 * buffer.h
 *
 *  Created on: Jul 19, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_BUFFER_H_
#define SRC_CORE_JET_BUFFER_H_

#include "jet.h"

//typedef enum {
//    JMBUF_TYPE_NORMAL = 0,
//    JMBUF_TYPE_PKT,
//    JMBUF_TYPE_EVENT,
//} jmbuf_type_t;


// 由于一个segment可能需要存储大片数据（如4K YUV图像），所以segment len相关字段统一为32位（DPDK是16位）
struct jmbuf_s {
    jobject_t parent;
    //MARKER cacheline0;

    void *buf_addr;           /**< Virtual address of segment buffer. */
    //phys_addr_t buf_physaddr; /**< Physical address of segment buffer. */

    uint32_t buf_len;         /**< Length of segment buffer. */

    /* next 6 bytes are initialised on RX descriptor rearm */
    //MARKER8 rearm_data;
    uint32_t data_off;

    /**
    * 16-bit Reference counter.
    * It should only be accessed using the following functions:
    * rte_mbuf_refcnt_update(), rte_mbuf_refcnt_read(), and
    * rte_mbuf_refcnt_set(). The functionality of these functions (atomic,
    * or non-atomic) is controlled by the CONFIG_RTE_MBUF_REFCNT_ATOMIC
    * config option.
    */
    union {
        /*rte_atomic16_t*/ uint16_t refcnt_atomic; /**< Atomically accessed refcnt */
        uint16_t refcnt;              /**< Non-atomically accessed refcnt */
    };
    uint8_t nb_segs;          /**< Number of segments. */
    //uint8_t port;             /**< Input port. */

    uint64_t ol_flags;        /**< Offload features. */

    /* remaining bytes are set on RX when pulling packet from descriptor */
    //MARKER rx_descriptor_fields1;

    /*
    * The packet type, which is the combination of outer/inner L2, L3, L4
    * and tunnel types.
    */
    //union {
    //    uint32_t packet_type; /**< L2/L3/L4 and tunnel information. */
    //    struct {
    //        uint32_t l2_type : 4; /**< (Outer) L2 type. */
    //        uint32_t l3_type : 4; /**< (Outer) L3 type. */
    //        uint32_t l4_type : 4; /**< (Outer) L4 type. */
    //        uint32_t tun_type : 4; /**< Tunnel type. */
    //        uint32_t inner_l2_type : 4; /**< Inner L2 type. */
    //        uint32_t inner_l3_type : 4; /**< Inner L3 type. */
    //        uint32_t inner_l4_type : 4; /**< Inner L4 type. */
    //    };
    //};

    uint32_t pkt_len;         /**< Total pkt len: sum of all segments. */
    uint32_t data_len;        /**< Amount of data in segment buffer. */
    //uint16_t vlan_tci;        /**< VLAN Tag Control Identifier (CPU order) */

    //union {
    //    uint32_t rss;     /**< RSS hash result if RSS enabled */
    //    struct {
    //        union {
    //            struct {
    //                uint16_t hash;
    //                uint16_t id;
    //            };
    //            uint32_t lo;
    //            /**< Second 4 flexible bytes */
    //        };
    //        uint32_t hi;
    //        /**< First 4 flexible bytes or FD ID, dependent on
    //        PKT_RX_FDIR_* flag in ol_flags. */
    //    } fdir;           /**< Filter identifier if FDIR enabled */
    //    struct {
    //        uint32_t lo;
    //        uint32_t hi;
    //    } sched;          /**< Hierarchical scheduler */
    //    uint32_t usr;     /**< User defined tags. See rte_distributor_process() */
    //} hash;                   /**< hash information */

    //uint32_t seqn; /**< Sequence number. See also rte_reorder_insert() */

    //uint16_t vlan_tci_outer;  /**< Outer VLAN Tag Control Identifier (CPU order) */

    /* second cache line - fields only used in slow path or on TX */
    //MARKER cacheline1 __rte_cache_aligned;

    union {
        void *userdata;   /**< Can be used for external metadata */
        uint64_t udata64; /**< Allow 8-byte userdata on 32-bit */
    };

    //struct rte_mempool *pool; /**< Pool from which mbuf was allocated. */
    struct jmbuf_s *next;    /**< Next segment of scattered packet. */

    /* fields to support TX offloads */
    //union {
    //    uint64_t tx_offload;       /**< combined for easy fetch */
    //    struct {
    //        uint64_t l2_len : 7; /**< L2 (MAC) Header Length. */
    //        uint64_t l3_len : 9; /**< L3 (IP) Header Length. */
    //        uint64_t l4_len : 8; /**< L4 (TCP/UDP) Header Length. */
    //        uint64_t tso_segsz : 16; /**< TCP TSO segment size */

    //        /* fields for TX offloading of tunnels */
    //        uint64_t outer_l3_len : 9; /**< Outer L3 (IP) Hdr Length. */
    //        uint64_t outer_l2_len : 7; /**< Outer L2 (MAC) Hdr Length. */

    //        /* uint64_t unused:8; */
    //    };
    //};

    // tail segment is first node, efficient for append op
    // tail = 0: n1(ME) -> n2 -> n3 -> n4 -> n1/NULL
    // tail = 1: n1 -> n2 -> n3 -> n4(ME) -> n1 : must be a cycle list
    //uint32_t tail : 1;

    /** Size of the application private data. In case of an indirect
    * mbuf, it stores the direct mbuf private data size. */
    //uint16_t priv_size;

    /** Timesync flags for use with IEEE1588. */
    //uint16_t timesync;

    /* Chain of off-load operations to perform on mbuf */
    //struct rte_mbuf_offload *offload_ops;
} /*__rte_cache_aligned*/;


struct jmbuf_class_s {
    jobject_class_t parent;

};

J_DECLARE_CLASS(jmbuf);


#define JMBUF_IND_ATTACHED    (1ULL << 62) /**< Indirect attached mbuf */

#define jmbuf_data(m) ((uint8_t *)((m)->buf_addr) + (m)->data_off)
#define jmbuf_data_end(m) ((uint8_t *)jmbuf_data(m) + (m)->data_len)

jmbuf_t *
jmbuf_alloc(jobject_class_t *kls, uint32_t payload_size);

jmbuf_t *
//jmbuf_new(jpad_t *pad);
jmbuf_new(jobject_class_t *kls, jpad_t *pad);

void
jmbuf_free(jmbuf_t *b);

#define jmbuf_litcpy(tail, str) jmbuf_memcpy(tail, str, ARRAY_SIZE(str) - 1)

int
jmbuf_strcpy(jmbuf_t **tail, char *src);
int
jmbuf_memcpy(jmbuf_t **tail, const void *src, size_t n);
int
jmbuf_mbufcpy(jmbuf_t **tail, jmbuf_t *mb);

int
jmbuf_vslprintf(jmbuf_t **tail, size_t n, const char *fmt, va_list args);
int
jmbuf_sprintf(jmbuf_t **tail, const char *fmt, ...);
int
jmbuf_snprintf(jmbuf_t **tail, size_t n, const char *fmt, ...);

static jmbuf_t *
jmbuf_head(const jmbuf_t *mbuf)
{
    return mbuf->next;
}

jmbuf_t *
jmbuf_copy(jmbuf_t *mbuf);

jmbuf_t *
//jmbuf_clone(jmbuf_t *mbuf);
jmbuf_clone(jobject_class_t *kls, jmbuf_t *mbuf);

static uint32_t jmbuf_data_len(const jmbuf_t *m)
{
    return m->data_len;
}

static int jmbuf_is_contiguous(const jmbuf_t *m)
{
    return !!(m->nb_segs == 1);
}

uint8_t *
jmbuf_append(jmbuf_t *m, uint32_t len);
int
jmbuf_chain(jmbuf_t **pchain, jmbuf_t *tail);


#endif /* SRC_CORE_JET_BUFFER_H_ */
