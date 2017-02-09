#pragma once
#include <jet/jet.h>
#include "module.h"
#include <jet/pad.h>
#include <jet/element.h>

namespace jet {
namespace jcache {

class jcache : public element
{
public:
    jcache();
    ~jcache();


private:
    //static int on_mux_push(pad *pad, pad::push_req *req, pad::push_type type);
    static int on_data_push(pad *pad, pad::push_req *req, pad::push_type type);

protected:
    //pad::push_req pushin;
    //pad::push_req pushout;

    pad data_pad;
    pad::push_req data_pushout;
};

} // jcache
} // jet