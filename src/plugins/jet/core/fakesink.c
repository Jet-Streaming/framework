/*
 * fakesink.c
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */


#include "fakesink.h"


static void
inst_init(jfakesink_t *self)
{
    ;
}

static void
vtbl_init(jfakesink_class_t *self)
{
    jelement_class_t *elm;

    elm = (jelement_class_t *)self;
    elm->inst_size = sizeof(jfakesink_t);
    elm->name = "fakesink";
}

J_DEFINE_CLASS(fakesink, basesink, inst_init, vtbl_init);
