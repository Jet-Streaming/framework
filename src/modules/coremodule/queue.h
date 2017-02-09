/*
 * queue.h
 *
 *  Created on: Sep 15, 2015
 *      Author: root
 */

#ifndef SRC_PLUGINS_JET_CORE_QUEUE_H_
#define SRC_PLUGINS_JET_CORE_QUEUE_H_

#include <jet/jet.h>
#include <jet/element.h>
#include <jet/pad.h>
#include "plugin.h"
#include "queuepad.h"

struct jqueue_s {
    jelement_t parent;

    jqueuepad_t srcpad;
    jqueuepad_t sinkpad;
};

struct jqueue_class_s {
    jelement_class_t parent;

};

J_DECLARE_CLASS(jqueue);


#endif /* SRC_PLUGINS_JET_CORE_QUEUE_H_ */
