#include "ltcp.h"
#include "../tcp.h"
#include <lua/jlthread.h>

#include "../ostcp.h"

namespace jet {
namespace coremodule {

//ltcp::ltcp()
//{
//}
//
//ltcp ::~ltcp()
//{
//}


//static void
//listen_cb(jtcp_t* server, int status)
//{
//    jltcp_t *self;
//    lua_State *L;
//    int rc;
//    jltcp_t *client;
//    jtcp_t *tcp;
//    jlua_ctx_t *ctx;
//    void *p;
//
//    //self = (ltcp_t *)server;
//    //server = ((lelement_t *)self)->jet_inst;
//    self = server->userdata;
//
//    ctx = jlua_ctx();
//    L = ctx->L;
//    rc = lua_gc(L, LUA_GCCOLLECT, 0);
//
//    //lua_rawgeti(L, LUA_REGISTRYINDEX, ltcp->connection_cb);
//    //rc = lua_pcall(L, 0, 0, 0);
//
//    //lua_rawgeti(L, LUA_REGISTRYINDEX, ltcp->connection_cb);
//
//    // get function on_connection 
//    //lua_rawgeti(L, LUA_REGISTRYINDEX, ctx->elements_table);
//    //rc = lua_type(L, -1);
//    //assert(rc == LUA_TTABLE);
//    //traverse_table(L, -1);
//
//    //co = lua_newthread(L);
//    //lua_pushthread(L);
//
//    //lua_pushlightuserdata(L, server);
//    //lua_rawget(L, -3);
//    //rc = lua_type(L, -1);
//    //p = lua_touserdata(L, -1);
//    //assert(p == server);
//
//    //lua_pushstring(L, "on_connect");
//    //lua_gettable(L, -2);
//    //rc = lua_type(L, -1);
//    //assert(rc == LUA_TFUNCTION);
//
//    //if (LUA_NOREF == self->on_connect) {
//    //    // reject connection
//    //    return;
//    //}
//    //lua_rawgeti(L, LUA_REGISTRYINDEX, self->on_connect);
//    rc = jlelement_push_listener((jlelement_t *)self, "connect");
//
//    // arg1: client
//    client = lua_newuserdata(L, sizeof(jltcp_t));
//    client->parent.jet_inst_class = J_CLASS(server);
//    jobject_init(jltcp_class(), client);
//    tcp = ((jlelement_t *)client)->jet_inst;
//    rc = jtcp_accept(tcp, server);
//    if (rc) {
//        return;
//    }
//    
//    //rc = jtcp_read_start(jtcp);
//
//    // set metatable
//    //lua_pushlightuserdata(L, ((jobject_t *)client)->_class);
//    //lua_rawget(L, LUA_REGISTRYINDEX);
//
//    // add in elements_table
//
//
//    //lua_xmove(L, co, 2);
//    //rc = lua_resume(co, 1);
//    rc = lua_pcall(L, 1, 0, 0);
//    //lua_call(L, 1, 0);
//    //rc = jlthread_pcall(L, 1, 0);
//    report(L, rc);
//    if (rc) {
//        // close ltcp
//    }
//    rc = lua_status(L);
//
//    rc = lua_gc(L, LUA_GCCOLLECT, 0);
//}


// listen(this, host, backlog, callback)
static int
llisten(lua_State *L)
{
    //int n;
    //int rc;
    //jltcp_t *self;
    //jtcp_t *tcp;
    //const char *addr;
    //size_t hostlen;
    //lua_Integer port, backlog;
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

    ////luaL_checkudata
    //self = luaL_checkudata(L, 1, ((jobject_class_t *)jltcp_class())->tname);
    ////self = lua_touserdata(L, 1);
    ////if (!self || J_CLASS(self) != &jtcp_class) {
    ////    return luaL_error(L, "this expected");
    ////}

    //
    //addr = luaL_checklstring(L, 2, &hostlen);
    ////if (!addr || hostlen > sizeof("255.255.255.255") - 1) {
    ////    return luaL_error(L, "host:port expected");
    ////}

    //port = luaL_checkinteger(L, 3);
    ////if (port < 0 || port > 65535) {
    ////    return luaL_error(L, "port expected");
    ////}

    //backlog = luaL_checkinteger(L, 4);
    ////if (backlog < 0 || backlog > 65535) {
    ////    return luaL_error(L, "backlog expected");
    ////}

    ////if (!lua_isfunction(L, 5)) {
    ////    return luaL_error(L, "callback expected");
    ////}
    ////ltcp = malloc(sizeof(ltcp_t));
    ////lua_pushvalue(L, 5);
    ////ltcp->connection_cb = luaL_ref(L, LUA_REGISTRYINDEX);
    ////ltcp->L = L;
    ////self->userdata = ltcp;
    ////lua_pushvalue(L, 1);
    ////ltcp->self = luaL_ref(L, LUA_REGISTRYINDEX);

    //tcp = ((jlelement_t *)self)->jet_inst;
    ////rc = offsetof(lelement_t, jet_inst);
    //tcp->userdata = self;
    //rc = jtcp_bind(tcp, addr, port);
    //rc = jtcp_listen(tcp, backlog, listen_cb);
    //if (EOK != rc) {
    //    goto out;
    //}

    //assert(LUA_NOREF == self->ref);
    //lua_pushvalue(L, 1);
    //self->ref = luaL_ref(L, LUA_REGISTRYINDEX);
//
//out:
//    lua_pushnumber(L, rc);
    return 1;
}


//static void 
//on_connect(jtcp_t* client, int status)
//{
//    jltcp_t *self;
//    int rc;
//    lua_State *co;
//
//    self = client->userdata;
//    rc = LUA_YIELD;
//    co = client->userdata;
//    assert(co);
//    //rc = lua_status(co);
//    rc = jlthread_resume(co, 0);
//}

static int
lconnect(lua_State *L)
{
    //int rc;
    //jltcp_t *self;
    //jtcp_t *tcp;
    //const char *ip;
    //unsigned short port;

    //self = luaL_checkudata(L, 1, "tcp");
    //ip = luaL_checkstring(L, 2);
    //port = luaL_checkinteger(L, 3);

    //tcp = ((jlelement_t *)self)->jet_inst;
    //tcp->userdata = self;

    //rc = jtcp_connect(tcp, ip, port, on_connect);
    //if (rc) {
    //    lua_pushnil(L);
    //    lua_pushstring(L, "error string");
    //    return 2;
    //}

    ////self->L = L;
    ////rc = lua_pushthread(L);
    ////self->on_connect = luaL_ref(L, LUA_REGISTRYINDEX);
    //tcp->userdata = L;

    return lua_yield(L, 0);
}


static int
lsend(lua_State *L)
{
    return 0;
}


static int
lreceive(lua_State *L)
{
    return 0;
}


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
__newindex(lua_State *L)
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
__tostring(lua_State *L)
{
    //jltcp_t *self;

    //self = lua_touserdata(L, 1);
    //lua_pushfstring(L, "BSDTCP: %d", self->tcp.socket);
    return 1;
}



static int
_gc(lua_State *L)
{
    //jltcp_t *self;

    //self = lua_touserdata(L, 1);

    return 0;
}



//static int
//inst_init(jltcp_t *self, va_list args)
//{
//    jelement_t *elm;
//    int rc;
//    lua_State *L;
//    jobject_class_t *obj;
//
//    elm = (jelement_t *)self;
//
//    //self->L = NULL;
//    //self->on_connect = LUA_NOREF;
//    self->ref = LUA_NOREF;
//
//    obj = jltcp_class();
//    //L = va_arg(args, lua_State *);
//    L = jlua_ctx()->L;
//    luaL_getmetatable(L, obj->tname);
//    lua_setmetatable(L, -2);
//
//    return EOK;
//}

//static void
//inst_fina(jltcp_t *self)
//{
//    lua_State *L;
//
//    L = jlua_ctx()->L;
//    luaL_unref(L, LUA_REGISTRYINDEX, self->ref);
//}

//static jproperty_t props[] = {
//    {
//        .name = "listen",
//        .description = "start listening on a TCP port",
//        .unwritable = 1,
//        .type = PROP_METHOD,
//        //        .method = {
//        //            .type = "",
//        //            .entry = _listen,
//        //        },
//    },
//};

static int
//vtbl_init(jltcp_class_t *self)
init(lua_State *L)
{
    //jtcp_class_t *tcp;
    //jlelement_class_t *elm;
    //jobject_class_t *obj, *pobj;
    int rc;
    //const struct luaL_Reg *l;
    //jlua_ctx_t *ctx;
    luaL_Reg lib[] = {
        { "listen", llisten },
        { "connect", lconnect },
        { "send", lsend },
        { "receive", lreceive },
        //{ "on_connect", error_handler },
        //{ "on_receive", error_handler },
        //{ "on_close", error_handler },
        { NULL, NULL }
    };

    //obj = (jobject_class_t *)self;

    //elm = (jlelement_class_t *)self;
    //elm->inst_size = sizeof(jltcp_t);
    //elm->name = "ostcp";

    //tcp = (jtcp_class_t *)self;
    //L = jlua_ctx()->L;

#if 0
    // create meta table
    luaL_newmetatable(L, obj->tname);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    //lua_pushvalue(L, -1);
    lua_pushcfunction(L, __newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pushcfunction(L, _gc);
    lua_setfield(L, -2, "__gc");
    //lua_pushcfunction(L, __tostring);
    //lua_setfield(L, -2, "__tostring");
#endif

    lua_newtable(L);
    luaL_register(L, NULL, lib);
    lua_setfield(L, -2, "tcp");

    //l = lib;
    //for (; l->name; l++) {
    //    if (l->func) {
    //        lua_pushcfunction(L, l->func);
    //    } else {
    //        lua_pushnil(L);
    //    }
    //    lua_setfield(L, -(0 + 2), l->name);
    //}
    //lua_pushinteger(L, 1);
    //lua_setfield(L, -2, "read_start");


    //lua_pushcfunction(L, index);
    //lua_setfield(L, -2, "__index");
    //lua_pushcfunction(L, newindex);
    //lua_setfield(L, -2, "__newindex");
    //lua_rawset(L, LUA_REGISTRYINDEX);

    // set meta table's meta table
    //pobj = obj->parent_class;
    //luaL_getmetatable(L, pobj->tname);
    ////lua_traverse_table(L, -2);
    //lua_setmetatable(L, -2);
    //lua_pop(L, 1);

    return EOK;
}

//JET_C_CTOR(__init) {
//	lua::lua::register_init(init);
//}

} // coremodule
} // jet
