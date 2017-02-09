#include <lua/jlua.h>
#include "ltcp.h"

namespace jet {
namespace coremodule {

using namespace lua;

JLUA_EXPORT int
luaopen_coremodule_coremodule(lua_State *L)
{
    int rc;

    // common
    //if (mod_pending == nullptr) {
    //    return -ENOENT;
    //}
    //rc = module::load();
    //if (rc) {
    //    lua_pushliteral(L, "load failed");
    //    return 1;
    //}

    // specific
    //luaL_register(...);
	lua_newtable(L);
	rc = initializer::initialize(L);
	//rc = lua_gettop(L);
	//rc = lua_gettop(L);

    return 1;
}

} // coremodule
} // jet