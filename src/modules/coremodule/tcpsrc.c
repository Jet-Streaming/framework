/*
 * tcpsrc.c
 *
 *  Created on: Aug 23, 2015
 *      Author: root
 */

#include "tcpsrc.h"

static jerr_t
create(jbasesrc_t *src, jmbuf_t **buf);

static jerr_t
inst_init(jtcpsrc_t *self, va_list args)
{
    jelement_t *elm;

    elm = (jelement_t *)self;
    return J_OK;
}

static jproperty_t props[] = {
        {
//                .offset = offsetof(jtcpsrc_t, fd),
                .name = "fd",
        },
};

static void
vtbl_init(jtcpsrc_class_t *self)
{
    jelement_class_t *elm;
    jbasesrc_class_t *src;
    jobject_class_t *obj;

    elm = (jelement_class_t *)self;
    elm->name = "tcpsrc";
    elm->inst_size = sizeof(jtcpsrc_t);
//    elm->srccaps = caps;
//    elm->nsrccaps = ARRAY_SIZE(caps);

    src = (jbasesrc_class_t *)self;
    src->create = create;

    obj = (jobject_class_t *)self;
    obj->properties = props;

    //
//    jvideocap_xraw_init(&xraw_cap);
}

J_DEFINE_CLASS(jtcpsrc, jbasesrc, inst_init, vtbl_init);


static jerr_t
create(jbasesrc_t *src, jmbuf_t **buf)
{
    jerr_t rc;
    jtcpsrc_t *tsrc;

    tsrc = (jtcpsrc_t *)src;
//    rc = st_recv(tsrc->fd, buf, JTIME_INFINITE);

    return rc;
}
