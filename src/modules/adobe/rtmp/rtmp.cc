#include "rtmp.h"
#include "message.h"
#include "amf.h"

namespace jet {
namespace rtmp {

//static const int csid_amf_init = 3;
//static const int csid_amf = 5;
//static const int csid_audio = 6;
//static const int csid_video = 7;

rtmp::rtmp()
    : message_pad("message", this, on_message_push)
    , chunk_size(0)
{
    int rc;

    rc = message_pad.set_cap(pad::direction::DIN, "rtmp_message");
    assert(EOK == rc);
    rc = message_pad.set_cap(pad::direction::DOUT, "rtmp_message");
    assert(EOK == rc);

}

rtmp::~rtmp()
{}

int
rtmp::on_message_push(pad *pad, pad::push_req *req, pad::push_type type)
{

    return EOK;
}

} // rtmp
} // jet