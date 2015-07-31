/*
 * basesink.c
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */


#include "basesink.h"


static void
inst_init(jbasesink_t *self)
{
    ;
}

static void
vtbl_init(jbasesink_class_t *self)
{
}

J_DEFINE_CLASS(basesink, element, inst_init, vtbl_init);
