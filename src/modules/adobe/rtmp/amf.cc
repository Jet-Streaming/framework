#include "amf.h"
#include <jet/endian.h>

namespace jet {
namespace rtmp {

enum {
    S_UNKNOWN,
    S_AMF0_CMD,
    S_AMF3_CMD,

    S_AMF0_NUM1,
    S_AMF0_NUM2,
    S_AMF0_NUM3,
    S_AMF0_NUM4,
    S_AMF0_NUM5,
    S_AMF0_NUM6,
    S_AMF0_NUM7,
    S_AMF0_NUM8,

    S_AMF0_BOOLEAN,

    S_AMF0_STRING_LEN1,
    S_AMF0_STRING_LEN2,
    S_AMF0_STRING_DATA,

    S_AMF0_OBJECT,
    S_AMF0_OBJECT_END,
};

// amf0-file-format-specification.pdf

enum {
    // basic types (ch2.1)
    AMF0_NUMBER = 0x00,
    AMF0_BOOLEAN = 0x01,
    AMF0_STRING = 0x02,
    AMF0_OBJECT = 0x03,
    AMF0_MOVIECLIP = 0x04, // reserved, not supported
    AMF0_NULL = 0x05,
    AMF0_UNDEFINED = 0x06,
    AMF0_REFERENCE = 0x07,
    AMF0_ECMA_ARRAY = 0x08,
    AMF0_OBJECT_END = 0x09,
    AMF0_STRICT_ARRAY = 0x0a,
    AMF0_DATE = 0x0b,
    AMF0_LONG_STRING = 0x0c,
    AMF0_UNSUPPORTED = 0x0d,
    AMF0_RECORDSET = 0x0e, // reserved, not supported
    AMF0_XML_DOCUMENT = 0x0f,
    AMF0_TYPED_OBJECT = 0x10,
    AMF0_AVMPLUS_OBJECT = 0x11, // for switching to AMF3

    AMF3_XX = 111,
};

//typedef union {
//    uint16_t u16;
//    struct {
//        uint8_t u81;
//        uint8_t u82;
//    } u8;
//} ju16_t;
//
//typedef union {
//    double dbl;
//    uint64_t u64;
//    struct {
//        uint8_t u81;
//        uint8_t u82;
//        uint8_t u83;
//        uint8_t u84;
//        uint8_t u85;
//        uint8_t u86;
//        uint8_t u87;
//        uint8_t u88;
//    } u8;
//} jdouble_t;

//
//static enum amf_state
//amf0_type2state(enum jrtmp_amf0_type type)
//{
//    switch (type) {
//    case AMF0_NUMBER:      return S_AMF0_NUM1;
//    case AMF0_BOOLEAN:     return S_AMF0_BOOLEAN;
//    case AMF0_STRING:      return S_AMF0_STRING_LEN1;
//    case AMF0_OBJECT:      return S_AMF0_OBJECT;
//    case AMF0_OBJECT_END:  return S_AMF0_OBJECT_END;
//    default:               return S_UNKNOWN;
//    }
//}

amf_writer::amf_writer(packet *p, int cs)
    : pkt(p)
    //, key(false)
    , chunk_size(cs)
{
    assert(p);
}

amf_writer::~amf_writer()
{
}

int 
amf_writer::write(const void *p, int l)
{
    char *dst;
    int rc, l2;

    l2 = pkt->tailroom();
    if (l2 >= l) {
        dst = pkt->append(l);
        memcpy(dst, p, l);
        return EOK;
    }

    dst = pkt->append(l2);
    memcpy(dst, p, l2);
    p = (const char *)p + l2;
    l -= l2;

    packet *pkt2 = new packet;
    rc = pkt2->alloc(chunk_size);
    rc = pkt->chain(pkt2);
    dst = pkt2->append(l);
    memcpy(dst, p, l);
    pkt = pkt2;

    return EOK;
}

int
amf_writer::write_type(uint8_t t)
{
    return write(&t, sizeof(t));
}

int 
amf_writer::string_len(uint16_t len)
{
    len = htobe16(len);
    return write(&len, sizeof(len));
}

////////////////////////////////////////////////
int 
amf_writer::string(const char *str, size_t len, bool key)
{
    int rc;

    // TODO long string

    if (!key) {
        rc = write_type(AMF0_STRING);
    }
    rc = string_len(len);
    return write(str, len);
}

int 
amf_writer::null()
{
    return EOK;
}

int 
amf_writer::undefined()
{
    return EOK;
}

int 
amf_writer::number(double n)
{
    uint64_t *u = (uint64_t *)&n;
    *u = htobe64(*u);

    int rc = write_type(AMF0_NUMBER);
    return write(&n, sizeof(n));
}

int 
amf_writer::boolean(bool b)
{
    int rc = write_type(AMF0_BOOLEAN);
    return write(&b, sizeof(b));
}

int 
amf_writer::start_object()
{
    return write_type(AMF0_OBJECT);
}

int 
amf_writer::end_object()
{
    int rc = string_len(0);
    return write_type(AMF0_OBJECT_END);
}

int 
amf_writer::start_array()
{
    return EOK;
}

int 
amf_writer::end_array()
{
    return EOK;
}

////////////////////////////////////////

amf_reader::amf_reader(bool amf3)
    : state(amf3 ? S_AMF3_CMD : S_AMF0_CMD)
{
}

amf_reader::~amf_reader()
{}

int 
amf_reader::input(packet *pkt)
{

    uint8_t *p, *pe;
    uint16_t u16;
    int rc;

    p = (uint8_t *)pkt->data();
    pe = p + pkt->len();
    rc = EOK;
    while (p < pe || S_AMF0_OBJECT_END == state) {
        switch (state) {
        case S_AMF3_CMD:
            ++p; // start with a dummy byte
            state = S_AMF0_CMD;
            break;
        //case S_AMF0_CMD:
        //    if (self->amf_object_depth) {
        //        self->amf_object_v = !self->amf_object_v;
        //        if (!self->amf_object_v) {
        //            ctx->state = S_AMF0_STRING_LEN1;
        //            break;
        //        }
        //    }
        //    ctx->state = amf0_type2state(*p++);
        //    break;

            // number
        case S_AMF0_NUM1:
            number.u8.u81 = *p++;
            ++state;
            break;
        case S_AMF0_NUM2:
            number.u8.u82 = *p++;
            ++state;
            break;
        case S_AMF0_NUM3:
            number.u8.u83 = *p++;
            ++state;
            break;
        case S_AMF0_NUM4:
            number.u8.u84 = *p++;
            ++state;
            break;
        case S_AMF0_NUM5:
            number.u8.u85 = *p++;
            ++state;
            break;
        case S_AMF0_NUM6:
            number.u8.u86 = *p++;
            ++state;
            break;
        case S_AMF0_NUM7:
            number.u8.u87 = *p++;
            ++state;
            break;
        case S_AMF0_NUM8:
            number.u8.u88 = *p++;
            number.u64 = be64toh(number.u64);
            rc = on_number(number.d);
            state = S_AMF0_CMD;
            break;

            // boolean
        case S_AMF0_BOOLEAN:
            boolean = *p++;
            rc = on_boolean(boolean);
            state = S_AMF0_CMD;
            break;

            // string
        case S_AMF0_STRING_LEN1:
            slen.u8.u81 = *p++;
            ++state;
            break;
        case S_AMF0_STRING_LEN2:
            slen.u8.u82 = *p++;
            slen.u16 = be16toh(slen.u16);
            rc = on_string(nullptr, slen.u16);
            ++state;
            break;
        case S_AMF0_STRING_DATA:
            u16 = min(slen.u16, pe - p);
            if (u16) {
                rc = on_string((char *)p, u16);
                p += u16;
            }
            //else {
            //    slen = 0;
            //}

            slen.u16 -= u16;
            if (0 == slen.u16) {
                state = S_AMF0_CMD;
            }
            break;

        //    // object
        //case S_AMF0_OBJECT:
        //    if (self->amf_object_depth++ < 0) {
        //        return -EINVAL;
        //    }
        //    rc = self->amf_cb->on_object(self);
        //    ctx->state = S_AMF0_CMD;
        //    break;
        //case S_AMF0_OBJECT_END:
        //    if (self->amf_object_depth-- <= 0) {
        //        return -EINVAL;
        //    }
        //    rc = self->amf_cb->on_object_end(self);
        //    ctx->state = S_AMF0_CMD;
        //    break;

        default:
            rc = -EINVAL;
            break;
        }
        if (rc) {
            return rc;
        }
    }
    assert(p == pe);

    return EOK;
}

} // rtmp
} // jet