/*
 * pad.c
 *
 *  Created on: Jun 29, 2015
 *      Author: root
 */

#include "pad.h"
//#include "config.h"
#include "element.h"

namespace jet {

pad::pad(const char *_name, element *parent,
    push_callback _on_push)
    : parent_element(parent)
    , peer(nullptr)
    , on_push(_on_push)
{
    strncpy(name, _name, sizeof(name));
    parent->add_pad(this);
}

pad::~pad()
{
}

int 
pad::link(pad *peerpad)
{
    if (!peerpad) {
        return -EINVAL;
    }
    if (peer || peerpad->peer) {
        return -EISCONN;
    }
    if (!in_cap || !out_cap ||
        !peerpad->in_cap || !peerpad->out_cap) {
        return -EINVAL;
    }

    if (!out_cap->is_kind_of(peerpad->in_cap) ||
        !peerpad->out_cap->is_kind_of(in_cap)) {
        return -EINVAL;
    }

    peer = peerpad;
    peerpad->peer = this;
    return EOK;
}

int
pad::unlink(pad *peerpad)
{
    return EOK;
}

int 
pad::push(push_req *req, packet *pkt)
{
    int rc;

    if (!req) {
        return -EINVAL;
    }
    if (!peer) {
        return -ENOTCONN;
    }

    if (req->sender) {
        if (this == req->sender) {
            return -EINPROGRESS;
        } else {
            // forwarding
            assert(nullptr == pkt);
            assert(req->pkt);
        }
    } else {
        assert(pkt);
        assert(!req->pkt);
        req->sender = this;
        //req->acked = 0;
        req->canceled = 0;
        req->pkt = pkt;
        //req->refc = 1;
    }

    rc = peer->on_push(peer, req, PACKET);

    // TODO
    //if (EOK == rc) {
    //    jpad_ack_push(push, rc);
    //}
    //else if (-EINPROGRESS == rc) {
    //    rc = EOK;
    //}
    return rc;
}

void 
pad::ack_push(push_req *req)
{
    // TODO free req->pkt
    req->pkt = nullptr;

    //req->acked = 1;
    //req->pending = 0;
    if (req->canceled) {
        req->canceled = 0;
    } else {
        pad *sender = req->sender;
        req->sender = nullptr;
        sender->on_push(sender, req, ACK);
    }
}

int 
pad::cancel_push(push_req *req)
{
    req->canceled = 1;
    req->sender = nullptr;
    return EOK;
}

int 
pad::set_cap(direction dir, const rt_class *rtc)
{
    if (!rtc) {
        return -EINVAL;
    }
    if (direction::DIN == dir) {
        in_cap = rtc;
    } else {
        assert(direction::DOUT == dir);
        out_cap = rtc;
    }
    return EOK;
}

int
pad::set_cap(direction dir, const char *name)
{
    return set_cap(dir, rt_class::get(name));
}

} // jet