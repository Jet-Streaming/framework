/*
 * identity.h
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */

#ifndef SRC_PLUGINS_JET_CORE_IDENTITY_H_
#define SRC_PLUGINS_JET_CORE_IDENTITY_H_

#include <jet/jet.h>
#include <jet/basetransform.h>

typedef struct jidentity_s jidentity_t;
typedef struct jidentity_class_s jidentity_class_t;

struct jidentity_s {
    jbasetransform_t parent;
};

struct jidentity_class_s {
    jbasetransform_class_t parent;

};

J_DECLARE_CLASS(identity);


#endif /* SRC_PLUGINS_JET_CORE_IDENTITY_H_ */
