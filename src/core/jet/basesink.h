/*
 * basesink.h
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_BASESINK_H_
#define SRC_CORE_JET_BASESINK_H_

#include <jet/jet.h>
#include <jet/element.h>
#include <jet/pad.h>

struct jbasesink_s {
    jelement_t parent;

//    jpad_t sinkpad;
};

struct jbasesink_class_s {
    jelement_class_t parent;

};

J_DECLARE_CLASS(jbasesink);


#endif /* SRC_CORE_JET_BASESINK_H_ */
