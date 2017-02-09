/*
 * queuepad.h
 *
 *  Created on: Sep 21, 2015
 *      Author: root
 */

#ifndef SRC_PLUGINS_JET_CORE_QUEUEPAD_H_
#define SRC_PLUGINS_JET_CORE_QUEUEPAD_H_


#include <jet/jet.h>
#include <jet/pad.h>
#include "plugin.h"

struct jqueuepad_s {
    jpad_t parent;
};

struct jqueuepad_class_s {
    jpad_class_t parent;

};

J_DECLARE_CLASS(jqueuepad);


#endif /* SRC_PLUGINS_JET_CORE_QUEUEPAD_H_ */
