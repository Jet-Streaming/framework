/*
 * pipeline.c
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */


#include "pipeline.h"

static int
change_state(jelement_t *self, jelement_transition_t trans);

static int
inst_init(jpipeline_t *self, va_list args)
{
    return EOK;
}

static void
vtbl_init(jpipeline_class_t *self)
{
    jelement_class_t *elm;

    elm = (jelement_class_t *)self;
    elm->change_state = change_state;
}

J_DEFINE_CLASS(jpipeline, jbin, inst_init, vtbl_init);


static int
change_state(jelement_t *self, jelement_transition_t trans)
{
    jpipeline_t *pl;
    jelement_class_t *parent_cls;
    int rc;

    pl = (jpipeline_t *)self;

    parent_cls = (jelement_class_t *)&jbin_class;
    rc = parent_cls->change_state(self, trans);

    return EOK;
}
