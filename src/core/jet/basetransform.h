/*
 * basetransform.h
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_BASETRANSFORM_H_
#define SRC_CORE_JET_BASETRANSFORM_H_

#include <jet/jet.h>
#include <jet/element.h>


struct jbasetransform_s {
    jelement_t parent;
};

struct jbasetransform_class_s {
    jelement_class_t parent;

};

J_DECLARE_CLASS(basetransform);


#endif /* SRC_CORE_JET_BASETRANSFORM_H_ */
