#pragma once
#include "../rtmp.h"
#include <lua/jlelement.h>
#include <map>

namespace jet {
namespace rtmp {

class lrtmp : public rtmp, public lua::lelement
{
public:
    lrtmp();
    ~lrtmp();

    static int lcall(lua_State *L);
    static int lset_chunk_size(lua_State *L);

private:
    std::map<int, lua::thread *> transactions;
};


} // rtmp
} // jet