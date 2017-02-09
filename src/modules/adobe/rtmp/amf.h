#pragma once
#include "rtmp.h"
#include <jet/packet.h>

namespace jet {
namespace rtmp {

class amf_writer {

    //uint32_t amf3 : 1;
    //uint32_t obj_depth : 8;
    //uint32_t array_n : 8;
    packet *pkt;
    //bool key;
    int chunk_size;

    int write(const void *p, int l);
    int write_type(uint8_t t);
    int string_len(uint16_t len);

public:
    amf_writer(packet *pkt, int cs);
    ~amf_writer();

    int number(double n);
    int boolean(bool b);
    int string(const char *str, size_t len, bool key);
    int null();
    int undefined();

    int start_object();
    int end_object();

    int start_array();
    int end_array();
};

class amf_reader {
    uint8_t state;
    
    union {
        uint8_t boolean;
        union {
            double d;
            uint64_t u64;
            struct {
                uint8_t u81;
                uint8_t u82;
                uint8_t u83;
                uint8_t u84;
                uint8_t u85;
                uint8_t u86;
                uint8_t u87;
                uint8_t u88;
            } u8;
        } number;
        union {
            uint16_t u16;
            struct {
                uint8_t u81;
                uint8_t u82;
            } u8;
        } slen;
    };

public:
    amf_reader(bool amf3);
    ~amf_reader();

    int input(packet *pkt);

    virtual int on_number(double n) = 0;
    virtual int on_boolean(uint8_t b) = 0;
    virtual int on_string(const char *s, uint16_t l) = 0;

};

} // rtmp
} // jet