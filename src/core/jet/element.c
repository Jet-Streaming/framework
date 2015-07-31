/*
 * element.c
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */


#include "element.h"
#include "pad.h"



static void
inst_init(jelement_t *self)
{
    self->name = NULL;
    self->parent_element = NULL;
    jlist_init(&self->elements);
    jlist_init(&self->srcpads);
    jlist_init(&self->sinkpads);
}

static jerr_t
change_state(jelement_t *self, jelement_state_transition_t trans)
{
    return J_OK;
}

static void
vtbl_init(jelement_class_t *self)
{
    self->change_state = change_state;
}

J_DEFINE_CLASS(element, object, inst_init, vtbl_init);

jerr_t
jelement_link(jelement_t *srcelm, jelement_t *sinkelm)
{
    jpad_t *srcpad, *sinkpad;
    jerr_t rc;

//    if (!jobject_is_class(self, &jelement_class)) {
//        return -1;
//    }
//    if (!jobject_is_class(elm, &jelement_class)) {
//        return -1;
//    }

    if (srcelm->nsrcpads != 1) {
        return J_ERROR;
    }
    if (sinkelm->nsinkpads != 1) {
        return J_ERROR;
    }

//    srcpad = container_of(srcelm->srcpads, jpad_t, pads);

    rc = jpad_link(srcpad, sinkpad);

    return rc;
}

/*
 *  url: icc://localhost/videotestsrc/srcpad
 *       itc://localhost/
 *       ipc://192.168.144.197/h264enc/sinkpad
 *
 *       localhost/...  (maybe more than one upstream per cluster)
 *       localcluster/rtmp-muxer/srcpad/www.cntv.com/live/stream (only one per cluster)
 *       localnetwork/... (only one upstream per CDN)
 */
jerr_t
jelement_link_url(jelement_t *self, const char *addr)
{
    //jsk_connect();
    return J_OK;
}

jerr_t
jelement_set_state(jelement_t *self, jelement_state_t state)
{
    return J_OK;
}


//jerr_t
//jelement_register(jplugin_t *plugin, jelement_class_t *_class)
//{
//    return J_OK;
//}



//void
//jelement_virtual(jelement_t *self)
//{
//    jelement_class.virtual(self);
//}
