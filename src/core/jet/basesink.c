/*
 * basesink.c
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */


#include "basesink.h"

static jerr_t on_receive(jpad_t *pad, jmbuf_t *buf);

static jerr_t
inst_init(jbasesink_t *self, va_list args)
{
    jelement_t *elm;

    elm = (jelement_t *)self;
//    jpad_init(&self->sinkpad);
//    jelement_add_pad(elm, &self->sinkpad, 0);

//    self->sinkpad.on_receive = on_receive;

    return J_OK;
}

static void
vtbl_init(jbasesink_class_t *self)
{
}

J_DEFINE_CLASS(jbasesink, jelement, inst_init, vtbl_init);


static jerr_t
on_receive(jpad_t *pad, jmbuf_t *buf)
{
    printf("basesink received %p\n", buf);
    return J_OK;
}
