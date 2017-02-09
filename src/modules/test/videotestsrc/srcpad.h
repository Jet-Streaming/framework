/*
 * srcpad.h
 *
 *  Created on: Sep 21, 2015
 *      Author: root
 */

#ifndef SRC_PLUGINS_JET_VIDEOTESTSRC_SRCPAD_H_
#define SRC_PLUGINS_JET_VIDEOTESTSRC_SRCPAD_H_

#include <jet/jet.h>
#include <jet/pad.h>

typedef struct jvideotestsrcpad_s jvideotestsrcpad_t;
typedef struct jvideotestsrcpad_class_s jvideotestsrcpad_class_t;

struct jvideotestsrcpad_s {
    jpad_t parent;
};

struct jvideotestsrcpad_class_s {
    jpad_class_t parent;

};


J_DECLARE_CLASS(jvideotestsrcpad);


#endif /* SRC_PLUGINS_JET_VIDEOTESTSRC_SRCPAD_H_ */
