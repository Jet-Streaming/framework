/*
 * srcpad.c
 *
 *  Created on: Sep 21, 2015
 *      Author: root
 */

#include "srcpad.h"

static jerr_t
inst_init(jvideotestsrcpad_t *self, va_list args)
{
    return J_OK;
}

static void
vtbl_init(jvideotestsrcpad_class_t *self)
{
}

J_DEFINE_CLASS(jvideotestsrcpad, jpad, inst_init, vtbl_init);
