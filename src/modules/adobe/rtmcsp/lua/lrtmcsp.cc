#include "lrtmcsp.h"
#include <lua/jlthread.h>
#include <jet/sequence.h>

namespace jet {
namespace rtmcsp {

using namespace lua;

static const char *klass = "lrtmcsp";

lrtmcsp::lrtmcsp()
    : waiting_th(nullptr)
{
}

lrtmcsp::~lrtmcsp()
{
}


int 
lrtmcsp::on_event(uint8_t state)
{
    thread *th;
    int rc;

    switch (state) {
    case S_INVALID:
        rc = EOK;
        break;
    case S_HS_DONE:
    case S_SET_CHUNK_SIZE:
        th = waiting_th;
        assert(th);
        waiting_th = nullptr;
        rc = th->resume(0);
        break;
    default:
        assert(0);
        break;
    }

    return rc;
}

//int
//lrtmcsp::on_sent()
//{
//    lrtmcsp *self = static_cast<lrtmcsp *>(this);
//    if (!self->waiting_th) {
//        return EOK;
//    }
//    return EOK;
//}

//static parser_handlers
//handlers = {
//    on_handshake,
//    on_handshake_done,
//    on_sent,
//};

static int
lhandshake(lua_State *L)
{
    lrtmcsp *self;
    int rc;

    self = (lrtmcsp *)luaL_checkudata(L, 1, klass);
    if (self->waiting_th) {
        lua_pushinteger(L, -EBUSY);
        return 1;
    }

    rc = self->handshake(nullptr);
    if (rc) {
        lua_pushinteger(L, rc);
        return 1;
    }

    thread *th = thread::to_thread(L);
    self->waiting_th = th;
    return th->yield(0);
}

int
lrtmcsp::lset_chunk_size(lua_State *L)
{
    lrtmcsp *self = (lrtmcsp *)luaL_checkudata(L, 1, klass);
    if (self->waiting_th) {
        lua_pushinteger(L, -EBUSY);
        return 1;
    }

    int rc = self->set_chunk_size(luaL_checkinteger(L, 2));
    thread *th = thread::to_thread(L);
    self->waiting_th = th;
    return th->yield(0);
}

int
lrtmcsp::lchunk_size(lua_State *L)
{
    lrtmcsp *self = (lrtmcsp *)luaL_checkudata(L, 1, klass);
    lua_pushinteger(L, self->chunk_size);
    return 1;
}

static int
lset_ack_size(lua_State *L)
{
    return 0;
}


static int
lnew(lua_State *L)
{
    void *t = lua_newuserdata(L, sizeof(lrtmcsp));
    new (t)lrtmcsp;

    luaL_getmetatable(L, klass);
    assert(lua_istable(L, -1));
    lua_setmetatable(L, -2);

    return 1;
}

static int
linit_mt(lua_State *L)
{
    int rc = luaL_newmetatable(L, klass);
    if (1 != rc) {
        return -EEXIST;
    }

    const luaL_Reg lib[] = {
        { "handshake", lhandshake },
        { "set_chunk_size", lrtmcsp::lset_chunk_size },
        { "chunk_size", lrtmcsp::lchunk_size },
        { "set_ack_size", lset_ack_size },
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
    const luaL_Reg lib[] = {
        { "new", lnew },
        { NULL, NULL }
    };

    lua_newtable(L);
    luaL_register(L, NULL, lib);
    lua_setfield(L, -2, "rtmcsp");

    return linit_mt(L);
}

static initializer
li(linit);

JLUA_EXPORT int
luaopen_adobe_rtmcsp(lua_State *L)
{
    lua_newtable(L);

    int rc = initializer::initialize(L);
    if (rc) {
        lua_pop(L, 1);
        lua_pushinteger(L, rc);
    }

    return 1;
}


} // rtmcsp
} // jet