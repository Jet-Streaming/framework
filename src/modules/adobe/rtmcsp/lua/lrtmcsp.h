#pragma once
#include "../rtmcsp.h"
#include <lua/jlelement.h>

namespace jet {
namespace rtmcsp {

class lrtmcsp : public rtmcsp, public lua::lelement
{
public:
    lrtmcsp();
    ~lrtmcsp();

    static int lset_chunk_size(lua_State *L);
    static int lchunk_size(lua_State *L);

    int on_event(uint8_t state);

//private:
    lua::thread *waiting_th;
};


} // rtmcsp
} // jet