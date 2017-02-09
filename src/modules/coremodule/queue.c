/*
 * queue.c
 *
 *  Created on: Sep 15, 2015
 *      Author: root
 */


#include "queue.h"

//static jerr_t on_receive(jpad_t *pad, jmbuf_t *buf);

static jerr_t
inst_init(jqueue_t *self, va_list args)
{
    jelement_t *elm;

    elm = (jelement_t *)self;
    //jqueuepad_init(&self->srcpad);
    jelement_add_pad(elm, (jpad_t *)&self->srcpad);
    //jqueuepad_init(&self->sinkpad);
    jelement_add_pad(elm, (jpad_t *)&self->sinkpad);

    return J_OK;
}

static void
vtbl_init(jqueue_class_t *self)
{
    jelement_class_t *elm;

    elm = (jelement_class_t *)self;
    elm->inst_size = sizeof(jqueue_t);
    elm->name = "queue";
}

J_DEFINE_CLASS(jqueue, jelement, inst_init, vtbl_init);
