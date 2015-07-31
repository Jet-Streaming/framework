/*
 * videocap_xraw.h
 *
 *  Created on: Jul 4, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_VIDEOCAP_XRAW_H_
#define SRC_CORE_JET_VIDEOCAP_XRAW_H_

#include <jet/videocap.h>


struct jvideocap_xraw_s {
    jvideocap_t parent;
};

struct jvideocap_xraw_class_s {
    jvideocap_class_t parent;
};

J_DEFCLARE_CLASS(videocap_xraw);

#endif /* SRC_CORE_JET_VIDEOCAP_XRAW_H_ */
