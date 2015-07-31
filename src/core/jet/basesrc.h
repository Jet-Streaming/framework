/*
 * basesrc.h
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */

#ifndef JET_BASESRC_H_
#define JET_BASESRC_H_

#include <jet/element.h>
#include <jet/pad.h>


struct jbasesrc_s {
    jelement_t parent;

    jpad_t srcpad;
};

struct jbasesrc_class_s {
    jelement_class_t parent;

};

J_DECLARE_CLASS(basesrc);

#endif /* JET_BASESRC_H_ */
