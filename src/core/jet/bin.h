/*
 * bin.h
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */

#ifndef JET_BIN_H_
#define JET_BIN_H_

#include <jet/jet.h>
#include <jet/element.h>
#include <jet/list.h>


struct jbin_s {
    jelement_t parent;
    jlist_head_t messages;
    int nelements;
};

struct jbin_class_s {
    jelement_class_t parent;

};

J_DECLARE_CLASS(bin);

int jbin_add(jbin_t *self, jelement_t *elm);

#endif /* JET_BIN_H_ */
