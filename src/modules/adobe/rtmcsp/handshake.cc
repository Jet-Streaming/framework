#include "rtmcsp.h"
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <jet/endian.h>
#include <jet/stream.h>

namespace jet {
namespace rtmcsp {

struct hs0 {
    uint8_t version;
};

struct hs1_common {
    uint32_t time;
    uint32_t version;
};

struct hs1_simple {
    uint8_t random[1528];
};

struct hs1_complex0 {
    uint8_t key[764];
    uint8_t digest[764];
};

struct hs1_complex1 {
    uint8_t digest[764];
    uint8_t key[764];
};

struct hs1 {
    hs1_common common;
    union {
        hs1_simple simple;
        hs1_complex0 complex0;
        hs1_complex1 complex1;
    };
};

struct hs2 {
    uint32_t time;
    uint32_t time2;
    uint8_t random_echo[1528];
};

#define HS_KEYLEN                   SHA256_DIGEST_LENGTH

static void
make_digest(const uint8_t *key, size_t keylen, 
    uint8_t *buf, size_t buflen, 
    uint8_t *skip, uint8_t *dst)
{
    HMAC_CTX hmac;
    unsigned int len;
    uint8_t *bufe;

    HMAC_CTX_init(&hmac);
    HMAC_Init_ex(&hmac, key, (int)keylen, EVP_sha256(), NULL);

    bufe = buf + buflen;
    if (skip && buf <= skip && skip <= bufe) {
        if (skip != buf) {
            HMAC_Update(&hmac, buf, skip - buf);
        }
        if (bufe != skip + HS_KEYLEN) {
            HMAC_Update(&hmac, skip + HS_KEYLEN, bufe - skip - HS_KEYLEN);
        }
    }
    else {
        HMAC_Update(&hmac, buf, buflen);
    }

    len = 0;
    HMAC_Final(&hmac, dst, &len);
    HMAC_CTX_cleanup(&hmac);
}

static size_t
find_digest(uint8_t *buf, size_t buflen, 
    const uint8_t *key, size_t keylen, size_t base)
{
    size_t n, offs;
    uint8_t digest[HS_KEYLEN];
    uint8_t *p;

    offs = 0;
    for (n = 0; n < 4; ++n) {
        offs += buf[base + n];
    }
    offs = (offs % 728) + base + 4;
    p = buf + offs;

    make_digest(key, keylen, buf, buflen, p, digest);

    if (memcmp(digest, p, HS_KEYLEN) == 0) {
        return offs;
    }

    return -1;
}

static void
write_digest(uint8_t *buf, size_t buflen, 
    const uint8_t *key, size_t keylen, size_t base)
{
    size_t n, offs;
    uint8_t *p;

    offs = 0;
    for (n = 8; n < 12; ++n) {
        offs += buf[base + n];
    }
    offs = (offs % 728) + base + 12;
    p = buf + offs;

    make_digest(key, keylen, buf, buflen, p, p);
}

/* Handshake keys */
static const uint8_t
server_key[] = {
    'G', 'e', 'n', 'u', 'i', 'n', 'e', ' ', 'A', 'd', 'o', 'b', 'e', ' ',
    'F', 'l', 'a', 's', 'h', ' ', 'M', 'e', 'd', 'i', 'a', ' ',
    'S', 'e', 'r', 'v', 'e', 'r', ' ',
    '0', '0', '1',

    0xF0, 0xEE, 0xC2, 0x4A, 0x80, 0x68, 0xBE, 0xE8, 0x2E, 0x00, 0xD0, 0xD1,
    0x02, 0x9E, 0x7E, 0x57, 0x6E, 0xEC, 0x5D, 0x2D, 0x29, 0x80, 0x6F, 0xAB,
    0x93, 0xB8, 0xE6, 0x36, 0xCF, 0xEB, 0x31, 0xAE
};


static const uint8_t
client_key[] = {
    'G', 'e', 'n', 'u', 'i', 'n', 'e', ' ', 'A', 'd', 'o', 'b', 'e', ' ',
    'F', 'l', 'a', 's', 'h', ' ', 'P', 'l', 'a', 'y', 'e', 'r', ' ',
    '0', '0', '1',

    0xF0, 0xEE, 0xC2, 0x4A, 0x80, 0x68, 0xBE, 0xE8, 0x2E, 0x00, 0xD0, 0xD1,
    0x02, 0x9E, 0x7E, 0x57, 0x6E, 0xEC, 0x5D, 0x2D, 0x29, 0x80, 0x6F, 0xAB,
    0x93, 0xB8, 0xE6, 0x36, 0xCF, 0xEB, 0x31, 0xAE
};

static const size_t
server_full_keylen = sizeof(server_key);
static const size_t
server_partial_keylen = 36;

static const size_t
client_full_keylen = sizeof(client_key);
static const size_t
client_partial_keylen = 30;

static const uint8_t
server_version[4] = {
    0x0D, 0x0E, 0x0A, 0x0D
};
static const uint8_t
client_version[4] = {
    0x0C, 0x00, 0x0D, 0x0E
};


// TODO use openssl rand
static void
fill_random_buffer(uint8_t *b, ssize_t s)
{
    while (s-- > 0) {
        *b++ = rand();
    }
}

static void
create_challenge(const uint8_t version[4], 
    const uint8_t *key, size_t keylen, uint8_t *out)
{
    hs0 *hs0buf;
    hs1 *hs1buf;
    uint64_t now;

    hs0buf = (hs0 *)out;
    hs0buf->version = 0x03;

    hs1buf = (hs1 *)(out + sizeof(hs0));
    now = uv_now(uv_default_loop());
    hs1buf->common.time = htobe32((uint32_t)now);
    memcpy(&hs1buf->common.version, version, sizeof(hs1buf->common.version));
    fill_random_buffer(hs1buf->simple.random, sizeof(hs1buf->simple.random));

    write_digest((uint8_t *)hs1buf, sizeof(hs1), key, keylen, 0);
}

int
rtmcsp::parse_challenge(const uint8_t *in, uint8_t *out,
    const uint8_t *peerkey, size_t peerkeylen, 
    const uint8_t *key, size_t keylen)
{
    hs0 *hs0buf;
    hs1 *hs1buf;

    hs0buf = (hs0 *)in;
    if (0x03 != hs0buf->version) {
        return -EINVAL;
    }

    hs1buf = (hs1 *)(in + sizeof(hs0));
    peer_epoch = be32toh(hs1buf->common.time);
    if (0 == hs1buf->common.version) {
        //hs_simple = 1;
        return EOK;
    }

    assert(0);
    //hs_simple = 0;

    return EOK;
}

static void
create_response(uint8_t *out)
{
    fill_random_buffer(out, sizeof(hs2));

    //if (s->hs_digest) {
    //    p = b->last - NGX_RTMP_HANDSHAKE_KEYLEN;
    //    key.data = s->hs_digest;
    //    key.len = NGX_RTMP_HANDSHAKE_KEYLEN;
    //    if (ngx_rtmp_make_digest(&key, b, p, p, s->connection->log) != NGX_OK) {
    //        return NGX_ERROR;
    //    }
    //}
}

#if 0

int
rtmcsp_handshake(jrtmcsp_t *self, const jmbuf_t *mbuf)
{
    jmbuf_t *hsmb, *msgmb;
    uint8_t *p, *pb, *pe;
    size_t size;
    struct rtmp_hs0 *hs0;
    int rc;
    jpad_push_t *push;
    jrtmp_message_t *msg;

    pb = p = jmbuf_data(mbuf);
    pe = p + jmbuf_data_len(mbuf);

execute:
    assert(pe >= p);
    if (pe == p) {
        return EOK;
    }

    switch (self->state) {
    case S_RECV_C0C1:
        hsmb = self->hs_s0s1s2;
        if (hsmb && c0c1_len == jmbuf_data_len(hsmb)) {

            // C0
            hs0 = p;
            if (0x03 != hs0->version) {
                // 0x06(RTMPE): not supported
                return -EINVAL;
            }
            p += sizeof(struct rtmp_hs0);

            // C1
            rc = hs_parse_challenge(self, p,
                hs_client_key, hs_client_partial_keylen,
                hs_server_key, sizeof(hs_server_key));
            if (rc) {
                return rc;
            }
            if (self->hs_simple) {
                // TODO 如何使用mbuf生成数据？

                // append s2: s2 = s1
                jmbuf_t *s2 = jmbuf_clone(NULL, hsmb);
                s2->data_off += sizeof(struct rtmp_hs0);
                s2->data_len -= sizeof(struct rtmp_hs0);
                jmbuf_mbufcpy(&hsmb, s2);
                jmbuf_free(s2);

                rc = jpad_push(&self->cs_pad, jpad_new_push(cs_push_cb, 0), hsmb);
                hsmb = self->hs_s0s1s2 = NULL;
                if (rc) {
                    return rc;
                }
                self->state = S_SEND_S0S1S2;
                p += sizeof(struct rtmp_hs1);
            }
            break;
        }

        if (hsmb) {
            // gather incomplete bytes
            ;
        }
        else {
            if (pe - p == c0c1_len) {
                hsmb = self->hs_s0s1s2 = jmbuf_new(NULL, &self->cs_pad); //jmbuf_clone(mbuf);
                if (!hsmb) {
                    return -ENOMEM;
                }
                //hsmb->data_off += p - pb;
                //hsmb->data_len = c0c1_len;
                jmbuf_mbufcpy(&self->hs_s0s1s2, mbuf);
            }
            else if (pe - p < c0c1_len) {
                assert(0);
                //hsmb = self->hs1_mbuf = jmbuf_alloc(c0c1_len);
                hsmb = self->hs_s0s1s2 = jmbuf_new(NULL, &self->cs_pad);
                if (!hsmb) {
                    return -ENOMEM;
                }
                // go to gather
                break;
            }
            else {
                // 不符合协议规范
                return -EFAULT;
            }
        }
        break;
    case S_RECV_C2:
        assert(pe - p >= sizeof(struct rtmp_hs2));
        p += sizeof(struct rtmp_hs2);
        jmbuf_free(self->hs_s0s1s2);
        self->hs_s0s1s2 = NULL;
        self->state = S_HS_DONE;
        break;

        // TODO move to chunk.c

    }
}
#endif

//void
//rtmcsp::phs_pp_cb(uv_prepare_t* handle)
//{
//    rtmcsp *self = container_of(handle, rtmcsp, phs_pp);
//    int rc = self->phs->on_handshake_done(self);
//}


int
rtmcsp::handshake(packet *pkt)
{
    int rc;
    uint8_t *pktdata;
    uint32_t len;
    char *pb;
    packet *outpkt;

execute:
    switch (state) {
    case S_INVALID:
        if (pkt) {
            rc = on_event(S_RECV_C0C1);
            // parse c0c1, send s0s1
            break;
        }

        outpkt = new stream;
        if (rc = outpkt->alloc(sizeof(hs0) + sizeof(hs1))) {
            return rc;
        }
        pktdata = (uint8_t *)outpkt->append(sizeof(hs0) + sizeof(hs1));
        assert(pktdata);

        create_challenge(client_version, 
            client_key, client_partial_keylen,
            pktdata);
        rc = mux_pad.push(&mux_pushout, outpkt);
        //printf("rtmcsp: send c0c1\n");

        state = S_SEND_C0C1;
        break;

    case S_SEND_C0C1:
        state = S_RECV_S0S1;
        break;

    case S_RECV_S0S1:
        len = sizeof(hs0) + sizeof(hs1);
        if (hspkt) {
            // gather
            assert(len > hspkt->len());
            if (pkt->len() >= len - hspkt->len()) {
                len = len - hspkt->len();
                rc = hspkt->copy(pkt->data(), len);
                pktdata = (uint8_t *)hspkt->data();
            }
            else {
                len = pkt->len();
                rc = hspkt->copy(pkt->data(), len);
                break;
            }
        } else {
            if (pkt->len() >= len) {
                // process
                pktdata = (uint8_t *)pkt->data();
            } else {
                // init gather
                hspkt = new packet;
                rc = hspkt->alloc(len);
                len = pkt->len();
                rc = hspkt->copy(pkt->data(), len);
                break;
            }
        }

        parse_challenge(pktdata, nullptr, 
            server_key, server_partial_keylen,
            client_key, client_full_keylen);

        outpkt = new stream;
        if (rc = outpkt->alloc(sizeof(hs2))) {
            return rc;
        }
        pktdata = (uint8_t *)outpkt->append(sizeof(hs2));
        assert(pktdata);
        create_response(pktdata);
        rc = mux_pad.push(&mux_pushout, outpkt);
        //printf("rtmcsp: send c2\n");

        delete hspkt;
        hspkt = nullptr;
        state = S_RECV_S2_SEND_C2_1;
        break;

    case S_RECV_S2_SEND_C2_1:
    case S_RECV_S2_SEND_C2_2:
        len = sizeof(hs2);
        if (hspkt) {
            // gather
            assert(len > hspkt->len());
            if (pkt->len() >= len - hspkt->len()) {
                len = len - hspkt->len();
                rc = hspkt->copy(pkt->data(), len);
                pktdata = (uint8_t *)hspkt->data();
            } else {
                len = pkt->len();
                rc = hspkt->copy(pkt->data(), len);
                break;
            }
        } else if (pkt) {
            if (pkt->len() >= len) {
                // process
                pktdata = (uint8_t *)pkt->data();
            } else {
                // init gather
                hspkt = new packet;
                rc = hspkt->alloc(len);
                len = pkt->len();
                rc = hspkt->copy(pkt->data(), len);
                break;
            }
        }

        if (S_RECV_S2_SEND_C2_2 == state) {
            rc = on_event(S_HS_DONE);
        } else {
            rc = EOK;
        }
        //printf("rtmcsp: client handshake done\n");

        delete hspkt;
        hspkt = nullptr;
        ++state;
        break;
    default:
        assert(0);
        break;
    }

    if (pkt) {
        pb = pkt->adj(len);
        assert(pb);
        if (pkt->len()) {
            goto execute;
        }
    }
    return EOK;
}


} // rtmcsp
} // jet