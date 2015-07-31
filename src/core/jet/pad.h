/*
 * pad.h
 *
 *  Created on: Jun 29, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_PAD_H_
#define SRC_CORE_JET_PAD_H_

#include <jet/jet.h>
#include <jet/object.h>
#include <jet/list.h>
#include <jsk/jsk.h>


struct jpad_s {
    jobject_t parent;

    const char *name;
    jlist_head_t pads;
//    union {
//        jpad_t *pad;
//        int jsk;
//    } peer;
//
//    unsigned int peer_jsk : 1;
    jfd_t peer;
};

struct jpad_class_s {
    jobject_class_t parent;

};

J_DECLARE_CLASS(pad);

jerr_t jpad_link(jpad_t *src, jpad_t *sink);

#endif /* SRC_CORE_JET_PAD_H_ */
