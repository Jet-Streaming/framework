/*
 * plugin.c
 *
 *  Created on: Jul 4, 2015
 *      Author: root
 */

#include <jet/plugin.h>
#include "identity.h"
#include "fakesink.h"

static jerr_t
plugin_init()
{
    jidentity_class_init();
    jfakesink_class_init();

    return J_OK;
}

static jelement_class_t *elements[] = {
        (jelement_class_t *)&jidentity_class,
        (jelement_class_t *)&jfakesink_class,
        NULL
};

jplugin_t jcore_plugin = {
        .name = "core",
        .licence = "LGPL",
        .elements = elements,
        .init = plugin_init,
};
