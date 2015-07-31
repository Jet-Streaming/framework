/*
 * capability.h
 *
 *  Created on: Jul 4, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_CAPABILITY_H_
#define SRC_CORE_JET_CAPABILITY_H_


struct jcapability_s {
    jobject_t parent;
};

struct jcapability_class_s {
    jobject_class_t parent;
    const char *name; // or enum
};

#endif /* SRC_CORE_JET_CAPABILITY_H_ */
