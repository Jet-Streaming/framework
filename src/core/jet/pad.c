/*
 * pad.c
 *
 *  Created on: Jun 29, 2015
 *      Author: root
 */

#include "pad.h"

static void
inst_init(jpad_t *self)
{

    self->name = NULL;
    jlist_init(&self->pads);
}

static void
vtbl_init(jpad_class_t *self)
{
}

J_DEFINE_CLASS(pad, object, inst_init, vtbl_init);

jerr_t
jpad_link(jpad_t *src, jpad_t *sink)
{
    /*src->peer.pad = sink;
    src->peer_jsk = 0;

    sink->peer.pad = src;
    sink->peer_jsk = 0;*/

    return J_OK;
}

/*
 *  url: icc://localhost/videotestsrc/srcpad
 *       itc://localhost/
 *       ipc://192.168.144.197/h264enc/sinkpad
 */
jerr_t
jpad_link_url(jpad_t *self, const char *url)
{
    return J_OK;
}
