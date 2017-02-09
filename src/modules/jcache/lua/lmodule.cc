#include <lua/jlua.h>
#include "ljcache.h"

namespace jet {
namespace jcache {

using namespace lua;

JLUA_EXPORT int
luaopen_jcache(lua_State *L)
{
    int rc;

    lua_newtable(L);
    rc = initializer::initialize(L);

    return 1;
}

} // jcache
} // jet