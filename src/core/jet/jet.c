/*
 * jet.c
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */

#include "jet.h"
#include "plugin.h"
#include "object.h"
#include "element.h"
#include "bin.h"
#include "pipeline.h"
#include "basetransform.h"
#include "basesink.h"

static jerr_t
class_init()
{
    jobject_class_init();

    jelement_class_init();
    jbasesrc_class_init();
    jbasetransform_class_init();
    jbasesink_class_init();

    jbin_class_init();

    jpipeline_class_init();

    return J_OK;
}

jerr_t
jet_init()
{
    jerr_t rc;

    rc = class_init();
    rc = jplugin_load("/media/workspace/jsf/out/Default/lib.target/libcore.so");

    return J_OK;
}
