#pragma once

//# undef BUILDING_UV_SHARED
//# define USING_UV_SHARED 1

#include <jet/jet.h>
#include <jet/element.h>
#include <jet/pad.h>

namespace jet {
namespace rtmp {

//struct {
//    int(*on_begin)(jrtmp_t *self);
//    int(*on_end)(jrtmp_t *self);
//    int(*on_number)(jrtmp_t *self, double n);
//    int(*on_boolean)(jrtmp_t *self, int n);
//    int(*on_string)(jrtmp_t *self, const char *s, uint32_t len);
//    int(*on_object)(jrtmp_t *self);
//    int(*on_object_end)(jrtmp_t *self);
//} jrtmp_amf_callback_t;

class rtmp : public element
{
public:
    rtmp();
    ~rtmp();
    //int parse(packet *pkt);

protected:
    pad message_pad;
    pad::push_req message_pushout;
    int chunk_size;

    //jrtmp_amf_callback_t *amf_cb;
    //int8_t amf_object_depth;
    //uint32_t amf_object_v : 1;
    //jarray_t av_pads; // av forward lookup table: msid -> pad

private:
    static int on_message_push(pad *pad, pad::push_req *req, pad::push_type type);

};


} // rtmp
} // jet