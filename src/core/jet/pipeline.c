/*
 * pipeline.c
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */


#include "pipeline.h"


static void
jpipeline_inst_init(jpipeline_t *self)
{
}

static void
jpipeline_vtbl_init(jpipeline_class_t *self)
{
}

J_DEFINE_CLASS(pipeline, bin, jpipeline_inst_init, jpipeline_vtbl_init);
