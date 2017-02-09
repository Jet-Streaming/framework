/*
 * tcpsrc.h
 *
 *  Created on: Aug 23, 2015
 *      Author: root
 */

#ifndef SRC_PLUGINS_JET_CORE_TCPSRC_H_
#define SRC_PLUGINS_JET_CORE_TCPSRC_H_

#include <jet/jet.h>
#include <jet/basesrc.h>

typedef struct jtcpsrc_s jtcpsrc_t;
typedef struct jtcpsrc_class_s jtcpsrc_class_t;

struct jtcpsrc_s {
    jbasesrc_t parent;

//    st_fd_t fd;
    int (*on_disconnected)(jtcpsrc_t *self);
};

struct jtcpsrc_class_s {
    jbasesrc_class_t parent;

};


J_DECLARE_CLASS(jtcpsrc);


#endif /* SRC_PLUGINS_JET_CORE_TCPSRC_H_ */
