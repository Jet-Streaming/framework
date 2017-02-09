/*
 * task.c
 *
 *  Created on: Sep 20, 2015
 *      Author: root
 */

#include "task.h"

//#include "../st/handle.h"
#include "pad.h"
#include "element.h"
#include "event.h"

typedef struct {
    jarray_t pollfds; // st_pollfd_t
    jarray_t pollfd_datas; // jtask_pollfd_data_t
    jpad_t *pad;
    jelement_t *driver;
} jtask_data_t;

static jerr_t
do_change_state(jelement_transition_t trans)
{
    jerr_t rc;
//    jtask_data_t *tdata;
    jelement_t *elm;

//    tdata = st_this_task->private_data;
//
//    elm = tdata->pad->parent_element;
//    rc = jelement_change_state(elm, trans);

    return rc;
}

//static jerr_t
//do_event(jevent_t *e)
//{
//    jerr_t rc;
//
//    switch (e->type) {
//        //case JEVT_CHANGE_STATE:
//        //    //rc = do_change_state(e->data.change_state.t);
//        //    break;
//        default:
//            rc = J_EINVAL;
//            break;
//    }
//
//    return rc;
//}

//static jerr_t
//do_mbuf(jmbuf_t *mbuf)
//{
//    jerr_t rc;
//
//    switch (mbuf->type) {
//        case JMBUF_TYPE_EVENT:
//            rc = do_event(mbuf->data);
//            break;
//        default:
//            rc = J_EINVAL;
//            break;
//    }
//
//    return rc;
//}

//static jerr_t
//init_task(jpad_t *pad)
//{
//
//}

static void *
task(void *p)
{
    jerr_t rc;
//    st_pollfd_t *pfd;
    jelement_class_t *driver_cls;
    jtask_data_t *tdata;

//    tdata = st_this_task->private_data;
//    jarray_init(&tdata->pollfds, 4, sizeof(st_pollfd_t));
    tdata->pad = p;

    // add task self
//    rc = jtask_add_pollfd(st_this_task->fd, 0, do_mbuf);
    if (rc) {
        ;
    }


    for (;;) {
//        rc = st_poll(tdata->pollfds.elts, tdata->pollfds.nelts, 0);

    }

    return (void *)rc;
}

jerr_t
jtask_new(jpad_t *pad)
{
    jerr_t rc;
//    st_task_t tid;
//    st_attr_t attr;

//    rc = st_task_create(&pad->task, NULL, task, pad);
//    if (J_OK != rc) {
//        return rc;
//    }

    return J_OK;
}

//jerr_t
//jtask_call(st_task_t task, jmbuf_t *mbuf)
//{
//    jerr_t rc;
////    sockaddr_in a;
//
//    if (task == st_this_task) {
//        rc = do_mbuf(mbuf);
//    } else {
////        st_send(task, mbuf);
////        st_task_recv(task, mbuf);
//    }
//
//    return rc;
//}



