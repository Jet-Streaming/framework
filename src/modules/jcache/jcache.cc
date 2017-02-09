#include "jcache.h"

namespace jet {
namespace jcache {


jcache::jcache()
    : data_pad("data", this, on_data_push)
{
    int rc;
    const rt_class *rtc;

    rtc = rt_class::get("sequence");
    assert(rtc);
    rc = data_pad.set_cap(pad::direction::DIN, rtc);
    assert(EOK == rc);
    rc = data_pad.set_cap(pad::direction::DOUT, rtc);
    assert(EOK == rc);
}

jcache::~jcache()
{}


int
jcache::on_data_push(pad *pad, pad::push_req *req, pad::push_type type)
{
    return EOK;
}


} // jcache
} // jet