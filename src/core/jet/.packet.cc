/*
 * buffer.c
 *
 *  Created on: Jul 19, 2015
 *      Author: root
 */


#include "packet.h"
#include "pad.h"

namespace jet {

//segment::segment()
//{
//}
//
//segment::~segment()
//{
//}

packet::packet()
{
}

packet::~packet()
{
}

}

//static int
//inst_init(jmbuf_t *self, va_list args)
//{
//    //size_t s;
//
//    //s = va_arg(args, size_t);
//    //self->mbuf = malloc(sizeof(jmbuf_t) + s);
//    //if (!self->mbuf) {
//    //    return -ENOMEM;
//    //}
//    //self->mbuf->ref = 1;
//    //self->mbuf->buf_len = s;
//
//    //self->data = self->mbuf->buf;
//    //self->data_len = 0;
//
//    self->buf_addr = NULL;
//    self->buf_len = 0;
//    self->data_off = 0;
//    self->refcnt_atomic = 1;
//    self->nb_segs = 1;
//    self->ol_flags = 0;
//    self->pkt_len = 0;
//    self->data_len = 0;
//    self->next = self;
//
//    return EOK;
//}
//
//static void
//inst_fina(jmbuf_t *self)
//{
//    //jmbuf_t *mb;
//    //jmbuf_t *b, *b2;
//
//    //b = self;
//    //while (b) {
//    //    mb = b->mbuf;
//    //    if (0 == --mb->ref) {
//    //        free(mb);
//    //    }
//
//    //    b2 = b->next;
//    //}
//    //free(b);
//
//}
//
//static void
//vtbl_init(jmbuf_class_t *self)
//{
//}
//
//J_DEFINE_CLASS(jmbuf, jobject, inst_init, vtbl_init);
//
////void
////jmbuf_init(jmbuf_t *m)
////{
////}
//
//jmbuf_t *
//jmbuf_alloc(jobject_class_t *kls, uint32_t payload_size)
//{
//    jmbuf_t *m;
//
//    if (!kls) {
//        kls = jmbuf_class();
//    }
//
//    m = malloc(/*sizeof(jmbuf_t)*/ kls->inst_size + payload_size);
//    if (!m) {
//        return NULL;
//    }
//
//    jobject_init(kls, m);
//    m->buf_addr = (char *)m + sizeof(jmbuf_t);
//    m->buf_len = payload_size;
//    //m->data_off = 0;
//    //m->refcnt_atomic = 1;
//    //m->nb_segs = 1;
//    //m->ol_flags = 0;
//    //m->pkt_len = 0;
//    //m->data_len = 0;
//    //m->next = m;
//    return m;
//}
//
//jmbuf_t *
//jmbuf_new(jobject_class_t *kls, jpad_t *pad)
//{
//    jmbuf_t *m;
//    jpad_t *peer;
//    jpad_seginfo_t *si;
//    int rc;
//
//    peer = pad->peer;
//    rc = jpad_init_seginfo(peer);
//
//    si = &peer->seginfo;
//    m = jmbuf_alloc(kls, si->headroom + si->maxlen);
//    m->data_off = si->headroom;
//    //mb->data_len -= peer->seg_headroom + peer->seg_tailroom;
//
//    return m;
//}
//
//void 
//jmbuf_free(jmbuf_t *b)
//{
//}
//
//static void
//jmbuf_attach(jmbuf_t *mi, jmbuf_t *m)
//{
//}
//
//jmbuf_t *
//jmbuf_clone(jobject_class_t *kls, jmbuf_t *mbuf)
//{
//    jmbuf_t *mi;
//
//    mi = jmbuf_alloc(kls, 0);
//    if (!mi) {
//        return NULL;
//    }
//
//    mi->buf_addr = mbuf->buf_addr;
//    mi->buf_len = mbuf->buf_len;
//    mi->data_off = mbuf->data_off;
//    mi->data_len = mbuf->data_len;
//
//    mi->next = mi;
//    mi->nb_segs = 1;
//    mi->ol_flags = mbuf->ol_flags | JMBUF_IND_ATTACHED;
//
//    return mi;
//}
//
//jmbuf_t *
//jmbuf_copy(jmbuf_t *mbuf)
//{
//    return NULL;
//}
//
//
///*
//
//*/
////jmbuf_t *
////jbuffer_copy(jmbuf_t *b)
////{
////    return NULL;
////}
////
////jmbuf_t *
////jbuffer_deep_copy(jmbuf_t *b)
////{
////    return NULL;
////}
//
//// tail：尾指针
//int
//jmbuf_strcpy(jmbuf_t **tail, char *src)
//{
//    size_t n;
//
//    n = strlen(src);
//    jmbuf_memcpy(tail, src, n);
//}
//
//int
//jmbuf_memcpy(jmbuf_t **tail, const void *src, size_t n)
//{
//    // TODO alloc new mbuf if needed
//    memcpy(jmbuf_data(*tail), src, n);
//    (*tail)->data_len += n;
//}
//
//// 追加mb的内容到tail
//int
//jmbuf_mbufcpy(jmbuf_t **tail, jmbuf_t *mb)
//{
//    uint32_t n;
//
//    n = jmbuf_data_len(mb);
//    memcpy(jmbuf_data(*tail) + (*tail)->data_len, jmbuf_data(mb), n);
//    (*tail)->data_len += n;
//}
//
//int 
//jmbuf_vslprintf(jmbuf_t **tail, size_t n, const char *fmt, va_list args)
//{
//    int rc = jstr_vslprintf(jmbuf_data(*tail), (uint8_t *)-1, n, NULL, NULL, fmt, args);
//    (*tail)->data_len += rc;
//    return rc;
//}
//
//int
//jmbuf_sprintf(jmbuf_t **tail, const char *fmt, ...)
//{
//    va_list   args;
//    int rc;
//
//    va_start(args, fmt);
//    rc = jmbuf_vslprintf(tail, -1, fmt, args);
//    va_end(args);
//    return rc;
//}
//
//int
//jmbuf_snprintf(jmbuf_t **tail, size_t n, const char *fmt, ...)
//{
//    va_list   args;
//    int rc;
//
//    va_start(args, fmt);
//    rc = jmbuf_vslprintf(tail, n, fmt, args);
//    va_end(args);
//    return rc;
//}
//
//jmbuf_t *
//jmbuf_lastseg(jmbuf_t *m)
//{
//    return m;
//}
//
//uint32_t 
//jmbuf_headroom(const jmbuf_t *m)
//{
//    return m->data_off;
//}
//
//uint32_t 
//jmbuf_tailroom(const jmbuf_t *m)
//{
//    return m->buf_len - jmbuf_headroom(m) - m->data_len;
//}
//
//uint8_t *
//jmbuf_append(jmbuf_t *m, uint32_t len)
//{
//    uint8_t *tail;
//    jmbuf_t *m_last;
//
//
//    m_last = jmbuf_lastseg(m);
//    if (len > jmbuf_tailroom(m_last))
//        return NULL;
//
//    tail = (uint8_t *)m_last->buf_addr + m_last->data_off + m_last->data_len;
//    m_last->data_len = m_last->data_len + len;
//    m->pkt_len = (m->pkt_len + len);
//    return tail;
//}
//
//int 
//jmbuf_chain(jmbuf_t **pchain, jmbuf_t *tail)
//{
//    jmbuf_t *cur_tail, *head;
//
//    head = jmbuf_head(*pchain);
//
//    /* Check for number-of-segments-overflow */
//    if (head->nb_segs + tail->nb_segs >= 1 << (sizeof(head->nb_segs) * 8))
//        return -EOVERFLOW;
//
//    /* Chain 'tail' onto the old tail */
//    cur_tail = *pchain;
//    tail->next = cur_tail->next;
//    cur_tail->next = tail;
//    *pchain = tail;
//
//    /* accumulate number of segments and total length. */
//    head->nb_segs += tail->nb_segs;
//    head->pkt_len += tail->pkt_len;
//
//    /* pkt_len is only set in the head */
//    tail->pkt_len = tail->data_len;
//
//    return EOK;
//}