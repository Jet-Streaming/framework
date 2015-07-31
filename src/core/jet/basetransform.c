/*
 * basetransform.c
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */


#include "basetransform.h"


static void
inst_init(jbasetransform_t *self)
{
    ;
}

static void
vtbl_init(jbasetransform_class_t *self)
{
}

J_DEFINE_CLASS(basetransform, element, inst_init, vtbl_init);
