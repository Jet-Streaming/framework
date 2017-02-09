#pragma once
//#undef BUILDING_UV_SHARED
//#define USING_UV_SHARED 1

#include <memory>

#include <jet/jet.h>
#include <jet/element.h>
#include <jet/pad.h>
//#include "parser.h"
#include <adobe/rtmp/message.h>
#include <queue>

namespace jet {
namespace rtmcsp {

enum {
    S_INVALID = 0,

    // server
    S_RECV_C0C1,
    //S_SEND_S0S1S2,
    S_RECV_C2,

    // client
    S_RECV_S0S1,
    S_SEND_C0C1,
    S_RECV_S2_SEND_C2_1,
    S_RECV_S2_SEND_C2_2,
    S_HS_DONE,

    // chunk

    // chunk basic header
    S_CBH1 = S_HS_DONE,
    S_CBH2,
    S_CBH3,

    // chunk message header
    S_CMH_TIMESTAMP1,
    S_CMH_TIMESTAMP2,
    S_CMH_TIMESTAMP3,

    S_CMH_LENGTH1,
    S_CMH_LENGTH2,
    S_CMH_LENGTH3,

    S_CMH_TYPEID,

    S_CMH_STREAMID1,
    S_CMH_STREAMID2,
    S_CMH_STREAMID3,
    S_CMH_STREAMID4,

    // extended timestamp
    S_CEXT_TIMESTAMP1,
    S_CEXT_TIMESTAMP2,
    S_CEXT_TIMESTAMP3,
    S_CEXT_TIMESTAMP4,

    // chunk data
    S_CHUNK_DATA,

    // out state
    S_SET_CHUNK_SIZE,
};

enum {
    RTMP_MSG_SET_CHUNK_SIZE = 1,
    RTMP_MSG_ABORT = 2,
    RTMP_MSG_ACKNOWLEDGEMENT = 3,
    RTMP_MSG_WINDOW_ACK_SIZE = 5, // server bandwidth
    RTMP_MSG_SET_PEER_BANDWIDTH = 6, // client bandwidth
};

//class rtmcsp;
//struct parser_handlers {
//    int (*on_handshake)(rtmcsp *self);
//    int(*on_handshake_done)(rtmcsp *self);
//    int(*on_sent)(rtmcsp *self);
//    //int(*on_message)();
//};

class rtmcsp : public element
{
public:
    rtmcsp();
    ~rtmcsp();
    int handshake(packet *pkt);
    int set_chunk_size(uint32_t cs);

private:
    int send_message(pad::push_req *req, rtmp::rtmp_message *msg);
    int get_csid(uint8_t msid);
    //int get_msid(uint8_t csid);
    int handle_message(rtmp::rtmp_message *msg);

protected:
    //parser_handlers *phs;
    //uint32_t s0s1_sent : 1;
    //uint32_t s2_recved : 1;
    //uv_prepare_t phs_pp;
    //static void phs_pp_cb(uv_prepare_t* handle);

    pad mux_pad;
    pad::push_req mux_pushout;

    pad message_pad;
    pad::push_req message_pushout;

    int parse_challenge(const uint8_t *buf, uint8_t *out,
            const uint8_t *peerkey, size_t peerkeylen,
            const uint8_t *key, size_t keylen);
    int parse(packet *pkt);
    static int on_mux_push(pad *pad, pad::push_req *req, pad::push_type type);
    static int on_message_push(pad *pad, pad::push_req *req, pad::push_type type);

    virtual int on_event(uint8_t state) = 0;

    uint8_t state;
    uint8_t out_state;
    //std::shared_ptr<packet> hspkt;
    packet *hspkt;

    uint8_t csid_msid[32];
    uint8_t msid_csid[4];

    std::queue<pad::push_req *> req_q;

    uint32_t ack_seq_num;
    uint32_t peer_epoch;
    uint32_t hs_simple : 1;

    // parse state
    uint32_t fmt : 2;
    uint32_t csid : 17; // 3 - 65599
    union {
        uint32_t u24 : 24;
        struct {
            uint8_t u81;
            uint8_t u82;
            uint8_t u83;
        } u8;
    } mlen;
    union {
        uint32_t u32;
        struct {
            uint8_t u81;
            uint8_t u82;
            uint8_t u83;
            uint8_t u84;
        } u8;
    } timestamp;
    union {
        uint32_t u32;
        struct {
            uint8_t u81;
            uint8_t u82;
            uint8_t u83;
            uint8_t u84;
        } u8;
    } mstreamid;
    uint8_t mtypeid;

    uint32_t chunk_size : 16;
    uint32_t in_chunk_size : 16;
    uint32_t extended_timestamp : 1;
    uint32_t mlen_left : 24;

    rtmp::rtmp_message *msg;
    //void *message_handler;
};


} // rtmcsp
} // jet