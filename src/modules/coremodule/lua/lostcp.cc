#include "lostcp.h"
#include "../tcp.h"
#include <lua/jlthread.h>

#include "../ostcp.h"

namespace jet {
namespace coremodule {

using namespace lua;

static const char *klass = "lostcp";

lostcp::lostcp()
{
}

lostcp::~lostcp()
{
}

static int
lnew(lua_State *L)
{
    void *t;

    t = lua_newuserdata(L, sizeof(lostcp));
    new (t)lostcp;

    luaL_getmetatable(L, klass);
    assert(lua_istable(L, -1));
    lua_setmetatable(L, -2);

    return 1;
}

static void
on_listen(tcp* server, int status)
{
    lostcp *self, *client;
    int rc;

    self = static_cast<lostcp *>(server);
    //printf("%s\n", __FUNCTION__);
    //return;
    thread *th = self->waiting_thread;
    if (!th) {
        // nobody is waiting, just queue it
        return;
    }

    // client should be on top of stack
    lua_State *L = th->from_thread();
    client = (lostcp *)luaL_checkudata(L, -1, klass);
    rc = self->accept(client);
    self->waiting_thread = nullptr;
    rc = th->resume(1);
    if (rc) {
        ;
    }

//    rc = lua_gc(L, LUA_GCCOLLECT, 0);
}


// listen(this, host, backlog, callback)
static int
llisten(lua_State *L)
{
    //int n;
    int rc;
    lostcp *self;
    //jtcp_t *tcp;
    const char *hostaddr;
    size_t hostlen;
    lua_Integer port, backlog;
    ////ltcp_t *ltcp;

    ////for (n = 0;; ++n) {
    ////    const char *name = lua_getupvalue(L, 5, n);
    ////    if (!name) break;
    ////    puts(name);
    ////}

    ////n = lua_gettop(L); 
    ////if (n < 1) {
    ////    return luaL_error(L, "invalid arguments");
    ////}

    self = (lostcp *)luaL_checkudata(L, 1, klass);
    hostaddr = luaL_checklstring(L, 2, &hostlen);
    port = luaL_checkinteger(L, 3);
    backlog = luaL_checkinteger(L, 4);

    if (self->waiting_thread) {
        lua_pushnil(L);
        lua_pushliteral(L, "socket busy");
        return 2;
    }

    if (port > (lua_Integer)std::numeric_limits<uint16_t>::max) {
        return 2;
    }
    rc = self->bind(hostaddr, (uint16_t)port);
    if (backlog > (lua_Integer)std::numeric_limits<int>::max) {
        return 2;
    }
    rc = self->listen((int)backlog, on_listen);

    //TODO 保存self的一份引用，以至于不会被gc

    //out:
    //    lua_pushnumber(L, rc);
    lua_pushboolean(L, 1);
    return 1;
}

static int
laccept(lua_State *L)
{
    int rc;
    lostcp *self, *client;

    self = (lostcp *)luaL_checkudata(L, 1, klass);

    if (self->waiting_thread) {
        lua_pushnil(L);
        lua_pushliteral(L, "socket is busy");
        return 2;
    }

    rc = lnew(L);
    if (1 != rc) {
        lua_pushnil(L);
        lua_pushliteral(L, "new client failed");
        return 2;
    }
    client = (lostcp *)luaL_checkudata(L, -1, klass);

    rc = self->accept(client);
    if (UV_EAGAIN == rc) {
        thread *th = thread::to_thread(L);
        self->waiting_thread = th;
        return th->yield(0);
    } else if (rc) {
        lua_pushnil(L);
        lua_pushstring(L, uv_err_name(rc));
        return 2;
    }

    assert(EOK == rc);
    return 1;
}

static void
on_connect(tcp* client, int status)
{
    lostcp *self;
    int rc;
    thread *ctx;

    self = static_cast<lostcp *>(client);
    ctx = self->waiting_thread;
    assert(ctx);
    self->waiting_thread = nullptr;
    element *eee = client;
    rc = ctx->resume(0);
    if (rc) {
        self->abort();
        return;
    }
}

static int
lconnect(lua_State *L)
{
    int rc;
    lostcp *self;
    const char *ip;
    lua_Integer port;

    self = (lostcp *)luaL_checkudata(L, 1, klass);
    ip = luaL_checkstring(L, 2);
    port = luaL_checkinteger(L, 3);

    if (self->waiting_thread) {
        lua_pushinteger(L, -EBUSY);
        return 1;
    }

    if (port > (lua_Integer)std::numeric_limits<uint16_t>::max) {
        return 2;
    }
    rc = self->connect(ip, (uint16_t)port, on_connect);
    if (rc) {
        lua_pushinteger(L, rc);
        return 1;
    }

    thread *th = thread::to_thread(L);
    self->waiting_thread = th;
    return th->yield(0);
}


//static int
//lsend(lua_State *L)
//{
//    return 0;
//}
//
//
//static int
//lreceive(lua_State *L)
//{
//    return 0;
//}


//static int
//error_handler(lua_State *L)
//{
//    return luaL_error(L, "function expected");
//}
//
//static int
//empty_handler(lua_State *L)
//{
//    return 0;
//}

static int
l__newindex(lua_State *L)
{
    //int rc;
    //jltcp_t *self;
    //const char *key;

    ////rc = lua_gettop(L);
    ////rc = LUA_TBOOLEAN;
    ////rc = lua_type(L, 3); // v
    ////rc = lua_type(L, 2); // k
    ////rc = lua_type(L, 1); // table

    //self = luaL_checkudata(L, 1, ((jobject_class_t *)&jltcp_class)->tname);
    //key = luaL_checklstring(L, 2, NULL);

    ////if (!strcmp(key, "on_connect")) {
    ////    luaL_checktype(L, 3, LUA_TFUNCTION);
    ////    luaL_unref(L, LUA_REGISTRYINDEX, self->on_connect);
    ////    lua_pushvalue(L, 3);
    ////    self->on_connect = luaL_ref(L, LUA_REGISTRYINDEX);
    ////} else {
    //    lua_getmetatable(L, 1);
    //    lua_settable(L, -1);
    ////}

    return 0;
}

static int
l__tostring(lua_State *L)
{
    //jltcp_t *self;

    //self = lua_touserdata(L, 1);
    //lua_pushfstring(L, "BSDTCP: %d", self->tcp.socket);
    return 1;
}



static int
l__gc(lua_State *L)
{
    lostcp *self;

    self = (lostcp *)luaL_checkudata(L, 1, klass);
    self->abort();
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
        { "listen", llisten },
        { "accept", laccept },
        { "connect", lconnect },
        //{ "send", lsend },
        //{ "receive", lreceive },
        { "__gc", l__gc },
        { "__tostring", l__tostring },
        { NULL, NULL }
    };
    luaL_register(L, NULL, lib);

    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    //lua_pushcfunction(L, __newindex);
    //lua_setfield(L, -2, "__newindex");
    //lua_pushcfunction(L, _gc);
    //lua_setfield(L, -2, "__gc");
    //lua_pushcfunction(L, __tostring);
    //lua_setfield(L, -2, "__tostring");

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
    lua_setfield(L, -2, "tcp");

    rc = linit_mt(L);

    return rc;
}

static initializer 
li(linit);

} // coremodule
} // jet
