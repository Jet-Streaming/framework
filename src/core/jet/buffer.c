/*
 * buffer.c
 *
 *  Created on: Jul 19, 2015
 *      Author: root
 */


#include "buffer.h"


static void
inst_init(jbuffer_t *self)
{
}

static void
vtbl_init(jbuffer_class_t *self)
{
}

J_DEFINE_CLASS(buffer, object, inst_init, vtbl_init);
