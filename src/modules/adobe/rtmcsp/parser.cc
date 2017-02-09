
#include <openssl/hmac.h>
#include <openssl/sha.h>

#include <jet/endian.h>
#include "parser.h"
#include "rtmcsp.h"




int
rtmcsp_parse(jrtmcsp_t *self, const jmbuf_t *mbuf)
{
//    jmbuf_t *mb;
//    uint8_t *p, *pe;
//    size_t size;
//    struct rtmp_hs0 *hs0;
//    int rc;
//
//    mb = jmbuf_head(mbuf);
//    p = jmbuf_data(mb);
//    pe = p + mb->data_len;
//
//execute:
//    assert(pe >= p);
//    if (pe == p) {
//        return EOK;
//    }
//
//    switch (parser->state) {
//    case S_RECV_C0:
//        assert(pe - p >= sizeof(struct rtmp_hs0));
//        hs0 = p;
//        if (0x03 != hs0->version) {
//            // 0x06(RTMPE): not supported
//            return -EINVAL;
//        }
//        p += sizeof(struct rtmp_hs0);
//        parser->state = S_RECV_C1;
//        break;
//    case S_RECV_C1:
//        if (parser->hs1_mbuf && sizeof(struct rtmp_hs1) == jmbuf_data_len(parser->hs1_mbuf)) {
//            assert(jmbuf_is_contiguous(parser->hs1_mbuf));
//            rc = hs_parse_challenge(parser, p,
//                hs_client_key, hs_client_partial_keylen,
//                hs_server_key, sizeof(hs_server_key));
//            if (rc) {
//                return rc;
//            }
//            if (parser->hs_simple) {
//                ;
//            }
//            break;
//        }
//
//        if (parser->hs1_mbuf) {
//            // gather incomplete bytes
//            ;
//        }
//        else {
//            if (pe - p >= sizeof(struct rtmp_hs1)) {
//                parser->hs1_mbuf = jmbuf_clone(mbuf);
//                if (!parser->hs1_mbuf) {
//                    return -ENOMEM;
//                }
//                parser->hs1_mbuf->data_len = sizeof(struct rtmp_hs1);
//            }
//            else {
//                parser->hs1_mbuf = jmbuf_alloc(sizeof(struct rtmp_hs1));
//                if (!parser->hs1_mbuf) {
//                    return -ENOMEM;
//                }
//            }
//        }
//        break;
//    case S_RECV_C2:
//        break;
//    default:
//        assert(0);
//        break;
//    }
//    goto execute;


    return EOK;
}

//void
//rtmcsp_init_parser(struct rtmcsp_parser *p, int srv)
//{
//    //p->hs1_len = 0;
//}

