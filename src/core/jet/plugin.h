/*
 * plugin.h
 *
 *  Created on: Jul 4, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_PLUGIN_H_
#define SRC_CORE_JET_PLUGIN_H_

#include <jet/jet.h>

struct jplugin_s {
    const char *name;
    const char *desciption;
    const char *licence;
    uint32_t version;
    jelement_class_t **elements;

    jerr_t (*init)();
};

jelement_t *
jplugin_create_element(const char *name);


#endif /* SRC_CORE_JET_PLUGIN_H_ */
