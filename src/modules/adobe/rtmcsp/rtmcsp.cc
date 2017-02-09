#include "rtmcsp.h"
#include <jet/endian.h>

namespace jet {
namespace rtmcsp {

rtmcsp::rtmcsp()
    : mux_pad("mux", this, on_mux_push)
    , message_pad("message", this, on_message_push)
    , hspkt(nullptr)
    , peer_epoch(0)
    , state(S_INVALID)
    , extended_timestamp(0)
    , chunk_size(128)
    , in_chunk_size(128)
    //, s0s1_sent(0)
    //, s2_recved(0)
    , msg(nullptr)
    , ack_seq_num(0)
{
    int rc;

    rc = mux_pad.set_cap(pad::direction::DIN, "stream");
    assert(EOK == rc);
    rc = mux_pad.set_cap(pad::direction::DOUT, "stream");
    assert(EOK == rc);

    rc = message_pad.set_cap(pad::direction::DIN, "rtmp_message");
    assert(EOK == rc);
    rc = message_pad.set_cap(pad::direction::DOUT, "rtmp_message");
    assert(EOK == rc);

    msid_csid[0] = 3;
}

rtmcsp::~rtmcsp()
{
    delete hspkt;
}

int
rtmcsp::on_mux_push(pad *pad, pad::push_req *req, pad::push_type type)
{
    rtmcsp *self;
    int rc;
    packet *pkt = req->pkt;

    self = container_of(pad, rtmcsp, mux_pad);
    assert(pad::PACKET == type || pad::ACK == type);

    //do {
        if (self->state < S_HS_DONE) {
            rc = self->handshake(pkt);
        } else {
            if (pkt) {
                rc = self->parse(pkt);
            } else if (&self->mux_pushout == req) {
                // ping response
                // set_chunk_size done
                rc = self->on_event(self->out_state);
                self->out_state = S_INVALID;
            } else {
                self->message_pad.ack_push(req);
            }
        }
        if (rc < 0) {
            // error
            assert(0);
        }
    //} while (pkt && pkt->len());
    
    if (pad::PACKET == type) {
        pad::ack_push(req);
    }

    return EOK;
}

int
rtmcsp::get_csid(uint8_t msid)
{
    if (msid >= ARRAY_SIZE(msid_csid)) {
        return -EINVAL;
    }
    return msid_csid[msid];
}

int 
rtmcsp::send_message(pad::push_req *req, rtmp::rtmp_message *msg)
{
    uint8_t hdr[3 + 11];

    // first message header
    uint8_t *phdr = hdr;

    int csid = get_csid(msg->msid);
    if (csid < 0) {
        return csid;
    }
    *phdr++ = csid;

    *phdr++ = msg->timestamp >> 16;
    *phdr++ = msg->timestamp >> 8;
    *phdr++ = msg->timestamp;

    *phdr++ = msg->mlen >> 16;
    *phdr++ = msg->mlen >> 8;
    *phdr++ = msg->mlen;

    *phdr++ = msg->type;

    // little endian
    *phdr++ = msg->msid;
    *phdr++ = msg->msid >> 8;
    *phdr++ = msg->msid >> 16;
    *phdr++ = msg->msid >> 24;

    char *dst = msg->prepend(phdr - hdr);
    memcpy(dst, hdr, phdr - hdr);

    // remain message header

    //req_q.push(req);
    int rc = mux_pad.push(&mux_pushout, msg);

    return EOK;
}


int
rtmcsp::on_message_push(pad *pad, pad::push_req *req, pad::push_type type)
{

    if (pad::PACKET != type) {
        return EOK;
    }

    rtmcsp *self = container_of(pad, rtmcsp, message_pad);
    int rc = self->send_message(req, (rtmp::rtmp_message *)req->pkt);

    return EOK;
}

int 
rtmcsp::parse(packet *pkt)
{
    uint8_t *p, *pb, *pe;
    uint32_t size;
    int rc;
    char *pp;

    if (!pkt) {
        return EOK;
    }
    pb = p = static_cast<uint8_t *>(pkt->data());
    pe = p + pkt->len();

execute:
    assert(pe >= p);
    if (pe == p) {
        //pkt->adj(pkt->len());
        return EOK;
    }

    switch (state) {
    case S_CBH1:
        fmt = (*p >> 6) & 0x03;
        csid = *p++ & 0x3f;
        if (0 == csid || 1 == csid) {
            state = S_CBH2;
        }
        else {
            state = S_CMH_TIMESTAMP1;
        }
        break;
    case S_CBH2:
        if (0 == csid) {
            state = S_CMH_TIMESTAMP1;
        }
        else {
            assert(1 == csid);
            state = S_CBH3;
        }
        csid = 64;
        csid += *p++;
        break;
    case S_CBH3:
        csid += (uint32_t)256 * *p++;
        break;

    case S_CMH_TIMESTAMP1:
        if (3 == fmt) {
            state = S_CEXT_TIMESTAMP1;
            break;
        }
        timestamp.u32 = 0;
        timestamp.u8.u83 = *p++;
        state = S_CMH_TIMESTAMP2;
        break;
    case S_CMH_TIMESTAMP2:
        assert(3 != fmt);
        timestamp.u8.u82 = *p++;
        state = S_CMH_TIMESTAMP3;
        break;
    case S_CMH_TIMESTAMP3:
        assert(3 != fmt);
        timestamp.u8.u81 = *p++;
        extended_timestamp = timestamp.u32 == 0xffffff;
        state = S_CMH_LENGTH1;
        break;

    case S_CMH_LENGTH1:
        if (0 != fmt && 1 != fmt) {
            state = S_CMH_TYPEID;
            break;
        }
        mlen.u24 = 0;
        mlen.u8.u83 = *p++;
        state = S_CMH_LENGTH2;
        break;
    case S_CMH_LENGTH2:
        assert(0 == fmt || 1 == fmt);
        mlen.u8.u82 = *p++;
        state = S_CMH_LENGTH3;
        break;
    case S_CMH_LENGTH3:
        assert(0 == fmt || 1 == fmt);
        mlen.u8.u81 = *p++;
        mlen_left = mlen.u24;
        state = S_CMH_TYPEID;
        break;

    case S_CMH_TYPEID:
        if (0 == fmt || 1 == fmt) {
            mtypeid = *p++;
        }
        state = S_CMH_STREAMID1;
        break;

    case S_CMH_STREAMID1:
        if (0 != fmt) {
            state = S_CEXT_TIMESTAMP1;
            break;
        }
        mstreamid.u32 = 0;
        mstreamid.u8.u84 = *p++;
        state = S_CMH_STREAMID2;
        break;
    case S_CMH_STREAMID2:
        assert(0 == fmt);
        mstreamid.u8.u83 = *p++;
        state = S_CMH_STREAMID3;
        break;
    case S_CMH_STREAMID3:
        assert(0 == fmt);
        mstreamid.u8.u82 = *p++;
        state = S_CMH_STREAMID4;
        break;
    case S_CMH_STREAMID4:
        assert(0 == fmt);
        mstreamid.u8.u81 = *p++;
        state = S_CEXT_TIMESTAMP1;
        break;


    case S_CEXT_TIMESTAMP1:
        if (!extended_timestamp) {
            state = S_CHUNK_DATA;
            break;
        }
        timestamp.u32 = 0;
        timestamp.u8.u84 = *p++;
        state = S_CEXT_TIMESTAMP2;
        break;
    case S_CEXT_TIMESTAMP2:
        assert(extended_timestamp);
        timestamp.u8.u83 = *p++;
        state = S_CEXT_TIMESTAMP3;
        break;
    case S_CEXT_TIMESTAMP3:
        assert(extended_timestamp);
        timestamp.u8.u82 = *p++;
        state = S_CEXT_TIMESTAMP4;
        break;
    case S_CEXT_TIMESTAMP4:
        assert(extended_timestamp);
        timestamp.u8.u81 = *p++;
        state = S_CHUNK_DATA;
        break;

    case S_CHUNK_DATA:

        //pp = pkt->adj(p - pb);
        //assert(pp);

        size = min(pe - p, in_chunk_size);
        size = min(size, mlen_left);
        pp = pkt->adj(size);
        assert(pp);

        if (!msg) {
            msg = new rtmp::rtmp_message;
            rc = pkt->clone(msg);
            msg->timestamp = timestamp.u32;
            msg->mlen = mlen.u24;
            msg->msid = mstreamid.u32;
            msg->type = mtypeid;
        } else {
            // TODO clone pkt, pkt may = msg
            rc = msg->chain(pkt);
        }

        mlen_left -= size;
        p += size;
        if (0 == mlen_left) {
            rc = handle_message(msg);
            delete msg;
            msg = nullptr;
        } else if (p >= pe) {
            break;
        }
        state = S_HS_DONE;

        break;

    default:
        assert(0);
        break;
    }
    goto execute;
    assert(0);
    return EOK;
}

int 
rtmcsp::set_chunk_size(uint32_t cs)
{
    // TODO check
    chunk_size = cs;

    rtmp::rtmp_message *msg = new rtmp::rtmp_message;
    
    int rc = msg->alloc(sizeof(cs));
    char *dst = msg->append(sizeof(cs));
    cs = htobe32(cs);
    memcpy(dst, &cs, sizeof(cs));

    msg->timestamp = 0;
    msg->mlen = msg->len();
    msg->type = RTMP_MSG_SET_CHUNK_SIZE;
    msg->msid = 0;
    rc = send_message(&mux_pushout, msg);
    if (rc) {
        return rc;
    }
    out_state = S_SET_CHUNK_SIZE;
    return EOK;
}

int 
rtmcsp::handle_message(rtmp::rtmp_message *msg)
{
    uint32_t u32, *pu32;
    uint8_t u8, *pu8;
    int rc;

    pu32 = (uint32_t *)msg->data();
    u32 = be32toh(*pu32);

    switch (msg->type) {
    case RTMP_MSG_SET_CHUNK_SIZE:
        if (sizeof(u32) != msg->len()) {
            return -EINVAL;
        }
        if (u32 < 128 || u32 > 0xffffff) {
            return -EINVAL;
        }
        in_chunk_size = u32;
        break;
    case RTMP_MSG_ABORT:
        if (sizeof(u32) != msg->len()) {
            return -EINVAL;
        }
        assert(0);
        break;
    case RTMP_MSG_ACKNOWLEDGEMENT:
        if (sizeof(u32) != msg->len()) {
            return -EINVAL;
        }
         // sequence number
        if (u32 < ack_seq_num) {
            return -EINVAL;
        }
        ack_seq_num = u32;
        break;
    case RTMP_MSG_WINDOW_ACK_SIZE:
        u32 = u32;
        break;
    case RTMP_MSG_SET_PEER_BANDWIDTH:
        pu8 = (uint8_t *)pu32;
        u8 = *++pu8; // limit type
        break;
    default:
        rc = mux_pad.push(&mux_pushout, msg);
        break;
    }

    return EOK;
}


} // rtmcsp
} // jet