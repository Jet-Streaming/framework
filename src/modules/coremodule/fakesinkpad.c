/*
 * fakesinkpad.c
 *
 *  Created on: Sep 21, 2015
 *      Author: root
 */


#include "fakesinkpad.h"

//static jerr_t on_receive(jpad_t *pad, jmbuf_t *buf);

static jerr_t
inst_init(jfakesinkpad_t *self, va_list args)
{
//    jelement_t *elm;
//
//    elm = (jelement_t *)self;
//    jpad_init(&self->sinkpad);
//    jelement_add_pad(elm, &self->sinkpad, 0);
//    elm->task =

    return J_OK;
}

static void
vtbl_init(jfakesinkpad_class_t *self)
{
//    jelement_class_t *elm;
//
//    elm = (jelement_class_t *)self;
//    elm->inst_size = sizeof(jqueue_t);
//    elm->name = "queue";
}

J_DEFINE_CLASS(jfakesinkpad, jpad, inst_init, vtbl_init);
