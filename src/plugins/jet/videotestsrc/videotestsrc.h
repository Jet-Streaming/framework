/*
 * videotestsrc.h
 *
 *  Created on: Jun 29, 2015
 *      Author: root
 */

#ifndef SRC_PLUGINS_JET_VIDEOTESTSRC_VIDEOTESTSRC_H_
#define SRC_PLUGINS_JET_VIDEOTESTSRC_VIDEOTESTSRC_H_

#include <jet/jet.h>
#include <jet/basesrc.h>

typedef struct jvideotestsrc_s jvideotestsrc_t;
typedef struct jvideotestsrc_class_s jvideotestsrc_class_t;

struct jvideotestsrc_s {
    jbasesrc_t parent;
};

struct jvideotestsrc_class_s {
    jbasesrc_class_t parent;

};


J_DECLARE_CLASS(videotestsrc);

extern jplugin_t jvideotestsrc_plugin;


#endif /* SRC_PLUGINS_JET_VIDEOTESTSRC_VIDEOTESTSRC_H_ */
