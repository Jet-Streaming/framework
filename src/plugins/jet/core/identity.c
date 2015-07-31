/*
 * identity.c
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */


#include "identity.h"
#include <jet/basetransform.h>

static void
inst_init(jidentity_t *self)
{
    ;
}

static void
vtbl_init(jidentity_class_t *self)
{
    jelement_class_t *elm;

    elm = (jelement_class_t *)self;
    elm->inst_size = sizeof(jidentity_t);
    elm->name = "identity";
}

J_DEFINE_CLASS(identity, basetransform, inst_init, vtbl_init);
