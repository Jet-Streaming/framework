/*
 * event.h
 *
 *  Created on: Sep 8, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_EVENT_H_
#define SRC_CORE_JET_EVENT_H_

#include "buffer.h"
/*

play:
pause:
seek:

*/
typedef enum {
    //JEVT_CHANGE_STATE = 1,

    // error
    JEVT_BADF = -EBADF,

    JEVT_INVALID = 0,
    JEVT_DESTROY,

    JEVT_FLUSH_START,
    JEVT_FLUSH_STOP,
    JEVT_SEEK,
    JEVT_QOS,

    // ����������link padǰ���ͣ���event������Ϻ��ܻ����Э�̲������ɹ������link
    //JEVT_NEGO,
    // pad֮�䴫���ض����͵����ݣ���Ϊjbuffer�����ࣩ��jevent_t, jbuffer���ֽ�������jsample����Ƶ��������jframe��ԭʼͼ��YUV��RGB��������

} jevent_type_t;


typedef struct {
    off_t offset;
} jevent_seek_t;

typedef struct {
    int dummy;
} jevent_eos_t;


//struct jevent_s {
    //jmbuf_t parent;
    //jevent_type_t type;
    //union {
    //    //struct {
    //    //    jelement_transition_t t;
    //    //} change_state;
    //    //struct {
    //    //    int rc;
    //    //} reply;

    //    jevent_seek_t seek;
    //    jevent_eos_t eos;
    //} u;

    //uint32_t reply:1;
//};

//struct jevent_class_s {
//    jbuffer_class_t parent;
//};
//
//J_DECLARE_CLASS(jevent);

//static jmbuf_t *
//jevt_alloc_change_state(jelement_transition_t t)
//{
//    jmbuf_t *b;
//    //jevent_t *e;
//
//    //b = jbuffer_alloc(sizeof(jevent_t));
//    //b->type = JMBUF_TYPE_EVENT;
//
//    //e = (jevent_t *)b->data;
//    //e->type = JEVT_CHANGE_STATE;
//    //e->reply = 0;
//    //e->data.change_state.t = t;
//
//    return b;
//}

#endif /* SRC_CORE_JET_EVENT_H_ */
