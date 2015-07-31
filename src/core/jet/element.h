/*
 * element.h
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */

#ifndef SRC_CORE_ELEMENT_H_
#define SRC_CORE_ELEMENT_H_

#include <jet/object.h>
#include <jet/list.h>
#include <jet/hash.h>


struct jelement_s {
    jobject_t parent;
    const char *name;
    jelement_t *parent_element;
    jlist_head_t elements;

    jlist_head_t srcpads;
    int32_t nsrcpads;
    jlist_head_t sinkpads;
    int32_t nsinkpads;
};

enum jelement_state_e {
    JELEMENT_STATE_NULL,
    JELEMENT_STATE_READY,
    JELEMENT_STATE_PAUSED,
    JELEMENT_STATE_PLAYING,
};

enum jelement_state_transition_e {
    JELEMENT_STATE_NULL_TO_READY = (JELEMENT_STATE_NULL << 4) | JELEMENT_STATE_READY,
    JELEMENT_STATE_READY_TO_PAUSED = (JELEMENT_STATE_READY << 4) | JELEMENT_STATE_PAUSED,

};

struct jelement_class_s {
    jobject_class_t parent;

    const char *name;
    size_t inst_size;
    jplugin_t *plugin;
    jhlist_node_t elements;

    jcapability_t **srccaps;
    int nsrccaps;
    jcapability_t **sinkcaps;
    int nsinkcaps;

    jerr_t (*to_registry)(jelement_t *self, jregistry_t *reg);
    jerr_t (*change_state)(jelement_t *self, jelement_state_transition_t trans);
    jerr_t (*recv_handler)(jpad_t *pad, jbuffer_t *buf);
};

J_DECLARE_CLASS(element);


//jerr_t
//jelement_register(jplugin_t *plugin, jelement_class_t *_class);

jerr_t
jelement_set_state(jelement_t *self, jelement_state_t state);




#endif /* SRC_CORE_ELEMENT_H_ */
