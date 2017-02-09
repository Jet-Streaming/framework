/*
 * identity.c
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */


#include "identity.h"
#include <jet/basetransform.h>

static jerr_t on_receive(jpad_t *pad, jmbuf_t *buf);

static jerr_t
inst_init(jidentity_t *self, va_list args)
{
    jelement_t *elm;

    elm = (jelement_t *)self;

    jobject_init(&jpad_class, &self->srcpad);
    jelement_add_pad(elm, &self->srcpad);
//    self->srcpad.on_receive = on_receive;

    jobject_init(&jpad_class, &self->sinkpad);
    jelement_add_pad(elm, &self->sinkpad);
//    self->sinkpad.on_receive = on_receive;

    return J_OK;
}

static void
vtbl_init(jidentity_class_t *self)
{
    jelement_class_t *elm;

    elm = (jelement_class_t *)self;
    elm->inst_size = sizeof(jidentity_t);
    elm->name = "identity";

}

J_DEFINE_CLASS(jidentity, jbasetransform, inst_init, vtbl_init);


static jerr_t
on_receive(jpad_t *pad, jmbuf_t *buf)
{
    jidentity_t *idt;
    jpad_t *pad2;
    jerr_t rc;

    idt = (jidentity_t *)pad->parent_element;
    if (pad == &idt->srcpad) {
    } else if (pad == &idt->sinkpad) {
        pad2 = &idt->srcpad;
    } else {
        return J_EINVAL;
    }

    //rc = jpad_push(pad2, buf);

    return rc;
}


