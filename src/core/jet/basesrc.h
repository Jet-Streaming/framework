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

//    jpad_t srcpad;
};

struct jbasesrc_class_s {
    jelement_class_t parent;

    int (*create)(jbasesrc_t *src, jmbuf_t **buf);
    int (*alloc)(jbasesrc_t *src, jmbuf_t **buf);
    int (*fill)(jbasesrc_t *src, jmbuf_t **buf);
};

J_DECLARE_CLASS(jbasesrc);

#endif /* JET_BASESRC_H_ */
