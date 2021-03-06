/*
 * fakesink.h
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */

#ifndef SRC_PLUGINS_JET_CORE_FAKESINK_H_
#define SRC_PLUGINS_JET_CORE_FAKESINK_H_

#include <jet/jet.h>
#include <jet/basesink.h>
#include "plugin.h"
#include "fakesinkpad.h"

//typedef struct jfakesink_s jfakesink_t;
//typedef struct jfakesink_class_s jfakesink_class_t;

struct jfakesink_s {
    jbasesink_t parent;

    jfakesinkpad_t sinkpad;
};

struct jfakesink_class_s {
    jbasesink_class_t parent;

};

J_DECLARE_CLASS(jfakesink);


#endif /* SRC_PLUGINS_JET_CORE_FAKESINK_H_ */
