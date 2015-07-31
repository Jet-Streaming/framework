/*
 * bin.c
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */


#include "bin.h"
#include "log.h"


static void
inst_init(jbin_t *self)
{
    jlist_init(&self->messages);
    self->nelements = 0;
}

static jerr_t
change_state(jelement_t *self, jelement_state_transition_t trans)
{
    return J_OK;
}


static void
vtbl_init(jbin_class_t *self)
{
    jelement_class_t *elm;

    elm = (jelement_class_t *)self;
    elm->change_state = change_state;
}

J_DEFINE_CLASS(bin, element, inst_init, vtbl_init);

int
jbin_add(jbin_t *self, jelement_t *elm)
{
//    jerr_t rc;
    jelement_t *self_elm;

    self_elm = (jelement_t *)self;
    if (J_UNLIKELY(elm == self_elm)) {
        jlog_error(NULL, "cannot add self\n");
        return J_ERROR;
    }

    // element name must be unique
//    for (;;) {
//        ;
//    }

    if (elm->parent_element) {
        return J_ERROR;
    }
    elm->parent_element = self_elm;

    // add it
    jlist_add(&elm->elements, &self_elm->elements);
    ++self->nelements;

    return J_OK;
}
