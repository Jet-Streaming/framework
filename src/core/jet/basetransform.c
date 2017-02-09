/*
 * basetransform.c
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */


#include "basetransform.h"


static jerr_t
inst_init(jbasetransform_t *self, va_list args)
{
    return J_OK;
}

static void
vtbl_init(jbasetransform_class_t *self)
{
}

J_DEFINE_CLASS(jbasetransform, jelement, inst_init, vtbl_init);
