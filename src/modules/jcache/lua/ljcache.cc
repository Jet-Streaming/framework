#include "ljcache.h"
#include <lua/jlthread.h>
#include <jet/sequence.h>

namespace jet {
namespace jcache {

using namespace lua;

static const char *klass = "ljcache";

ljcache::ljcache()
{
}

ljcache::~ljcache()
{
}

static int
lnew(lua_State *L)
{
    void *t;

    t = lua_newuserdata(L, sizeof(ljcache));
    new (t)ljcache;

    luaL_getmetatable(L, klass);
    assert(lua_istable(L, -1));
    lua_setmetatable(L, -2);

    return 1;
}

static int
lopen(lua_State *L)
{
    return 0;
}

static int
linit_mt(lua_State *L)
{
    int rc;

    rc = luaL_newmetatable(L, klass);
    if (1 != rc) {
        return -EEXIST;
    }

    luaL_Reg lib[] = {
        { "open", lopen },
        //{ "__gc", l__gc },
        //{ "__tostring", l__tostring },
        { NULL, NULL }
    };
    luaL_register(L, NULL, lib);

    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    luaL_getmetatable(L, "lelement");
    assert(lua_istable(L, -1));
    lua_setmetatable(L, -2);

    lua_pop(L, 1);

    return EOK;
}

static int
linit(lua_State *L)
{
    int rc;
    luaL_Reg lib[] = {
        { "new", lnew },
        { NULL, NULL }
    };

    lua_newtable(L);
    luaL_register(L, NULL, lib);
    lua_setfield(L, -2, "jcache");

    rc = linit_mt(L);

    return rc;
}

static initializer
li(linit);

} // jcache
} // jet