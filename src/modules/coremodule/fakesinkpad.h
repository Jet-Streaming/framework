/*
 * fakesinkpad.h
 *
 *  Created on: Sep 21, 2015
 *      Author: root
 */

#ifndef SRC_PLUGINS_JET_CORE_FAKESINKPAD_H_
#define SRC_PLUGINS_JET_CORE_FAKESINKPAD_H_

#include <jet/jet.h>
#include <jet/pad.h>
#include "plugin.h"

struct jfakesinkpad_s {
    jpad_t parent;
};

struct jfakesinkpad_class_s {
    jpad_class_t parent;

};

J_DECLARE_CLASS(jfakesinkpad);


#endif /* SRC_PLUGINS_JET_CORE_FAKESINKPAD_H_ */
