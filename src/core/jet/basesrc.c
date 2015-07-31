/*
 * basesrc.c
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */

#include "basesrc.h"


static void
inst_init(jbasesrc_t *self)
{
    jelement_t *elm;

    elm = (jelement_t *)self;
    jpad_init(&self->srcpad);
    jlist_add(&self->srcpad.pads, &elm->srcpads);
}

static void
vtbl_init(jbasesrc_class_t *self)
{
}

J_DEFINE_CLASS(basesrc, element, inst_init, vtbl_init);
