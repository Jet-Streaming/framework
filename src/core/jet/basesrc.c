/*
 * basesrc.c
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */

#include "basesrc.h"
#include <jsk/jsk.h>

static jerr_t
change_state(jelement_t *self, jelement_transition_t trans);
static jerr_t
default_create(jbasesrc_t *src, jmbuf_t **buf);
static jerr_t
default_alloc(jbasesrc_t *src, jmbuf_t **buf);


static jerr_t
inst_init(jbasesrc_t *self, va_list args)
{
    jelement_t *elm;

    elm = (jelement_t *)self;

    return J_OK;
}

static void
vtbl_init(jbasesrc_class_t *self)
{
    jelement_class_t *elm;

    elm = (jelement_class_t *)self;
    elm->change_state = change_state;

    self->create = default_create;
    self->alloc = default_alloc;
}

J_DEFINE_CLASS(jbasesrc, jelement, inst_init, vtbl_init);


static void *
task(void *p)
{
    jbasesrc_t *src;
    jbasesrc_class_t *src_cls;
    jerr_t rc;
    jelement_t *elm;
    jmbuf_t *buf;
    jpad_t *pad;

    src = p;
    src_cls = ((jobject_t *)src)->_class;
    elm = p;
//    pad = container_of(elm->srcpads.next, jpad_t, pads);

    while (1) {
        rc = src_cls->create(src, &buf);
        //rc = jpad_push(pad, buf);
        jbuffer_free(buf);
    }

    return (void *)rc;
}

static jerr_t
change_state(jelement_t *self, jelement_transition_t trans)
{
    jerr_t rc;

    switch (trans) {
        case JELEMENT_STATE_NULL_TO_READY:
            break;
        case JELEMENT_STATE_READY_TO_PAUSED:
            break;
        case JELEMENT_STATE_PAUSED_TO_PLAYING:
//            assert(!self->task);
//            rc = st_task_create(&self->task, NULL, task, self);
//            if (J_OK != rc) {
//                return rc;
//            }
            break;
        default:
            assert(0);
            return J_EINVAL;
    }

    rc = jelement_class.change_state(self, trans);

    return J_OK;
}

static jerr_t
default_create(jbasesrc_t *src, jmbuf_t **buf)
{
    jerr_t rc;

    *buf = NULL;
    rc = jbasesrc_class.alloc(src, buf);
    if (J_OK != rc) {
        return rc;
    }

    assert(*buf);
    rc = jbasesrc_class.fill(src, buf);
    if (J_OK != rc) {
        return rc;
    }

    return J_OK;
}

static jerr_t
default_alloc(jbasesrc_t *src, jmbuf_t **buf)
{
//    *buf = jbuffer_alloc();
    return J_OK;
}









