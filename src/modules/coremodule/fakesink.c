/*
 * fakesink.c
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */


#include "fakesink.h"


static jerr_t
inst_init(jfakesink_t *self, va_list args)
{
    jelement_t *elm;

    elm = (jelement_t *)self;
    //jfakesinkpad_init(&self->sinkpad);
    jelement_add_pad(elm, (jpad_t *)&self->sinkpad);

    return J_OK;
}

static void
vtbl_init(jfakesink_class_t *self)
{
    jelement_class_t *elm;

    elm = (jelement_class_t *)self;
    elm->inst_size = sizeof(jfakesink_t);
    elm->name = "fakesink";
}

J_DEFINE_CLASS(jfakesink, jbasesink, inst_init, vtbl_init);
