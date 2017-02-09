
#include "jlelement.h"
#include "jlthread.h"

namespace jet {
namespace lua {

static const char *klass = "lelement";

lelement::lelement() :
    waiting_thread(nullptr)
{
}

lelement::~lelement()
{
}

static int
llink(lua_State *L)
{
    element *e1, *e2;
    const char *n1, *n2;
    int rc;

    //void *lh = luaL_checkudata(L, 1, "lostcp");
    //uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    e1 = (element *)lua_touserdata(L, 1);
    n1 = luaL_checkstring(L, 2);
    e2 = (element *)lua_touserdata(L, 3);
    n2 = luaL_checkstring(L, 4);
    rc = e1->link(n1, e2, n2);
    lua_pushinteger(L, rc);
    return 1;
}


static int
linit(lua_State *L)
{
    int rc;

    rc = luaL_newmetatable(L, klass);
    if (1 != rc) {
        return -EEXIST;
    }

    luaL_Reg lib[] = {
        { "link", llink },
        //{ "__gc", gc },
        //{ "__tostring", tostring },
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

    lua_pop(L, 1);

    return EOK;
}

static initializer 
li(linit, initializer::FRAMEWORK);

} // lua
} // jet


//int
//jlelement_add_listener(jlelement_t *self, lua_State *L)
//{
//    uint32_t i;
//    jlelement_event_t *e;
//    int rc;
//
//    e = self->events.elts;
//    for (i = 0; i != self->events.nelts; ++i, ++e) {
//        if (!strcmp(e->evt, event)) {
//            break;
//        }
//    }
//    if (i >= self->events.nelts) {
//        e = jarray_push(&self->events);
//        if (!e) {
//            return -ENOMEM;
//        }
//        e->evt = event;
//        //rc = jarray_init(&e->refs, 1, sizeof(int));
//        e->ref = LUA_NOREF;
//    }
//
//    return i;
//}

#if 0
static jlelement_event_t *
find_event(jlelement_t *self, const char *evt)
{
    uint32_t i;
    jlelement_event_t *e;

    e = self->events.elts;
    for (i = 0; i != self->events.nelts; ++i, ++e) {
        if (!strcmp(e->evt, evt)) {
            return e;
        }
    }
    return NULL;
}

int
jlelement_push_listener(jlelement_t *self, const char *event)
{
    lua_State *L;
    jlelement_event_t *e;
    //jlua_ctx_t *ctx;
    int rc;

    //if (event < 0 || event >= self->events.nelts) {
    //    return J_EINVAL;
    //}

    //e = self->events.elts;
    //e += event;
    e = find_event(self, event);
    if (!e || LUA_NOREF == e->ref) {
        return -EINVAL;
    }

    L = jlua_ctx()->L;
    lua_rawgeti(L, LUA_REGISTRYINDEX, e->ref);

    return EOK;
}

static int
link(lua_State *L)
{
    int rc, elmidx1, elmidx2;
    jlelement_t *elm1, *elm2;
    //size_t urllen;
    const char *pad1, *pad2;

    //n = lua_gettop(L);
    //if (n != 1) {
    //    return luaL_error(L, "invalid arguments");
    //}

    rc = elmidx1 = 1;
    elm1 = lua_touserdata(L, rc++); // -2
    if (!elm1 || !jobject_is_class(elm1, jlelement_class())) {
        return luaL_error(L, "lelement_class expected");
    }
    pad1 = lua_tolstring(L, rc, NULL);
    if (pad1) {
        ++rc;
    }

    elmidx2 = rc;
    elm2 = lua_touserdata(L, rc++); // -1
    if (!elm2 || !jobject_is_class(elm2, jlelement_class())) {
        return luaL_error(L, "lelement_class expected");
    }
    pad2 = lua_tolstring(L, rc, NULL);
    if (pad2) {
        ++rc;
    }

    rc = jelement_link(elm1->jet_inst, pad1, elm2->jet_inst, pad2);
    if (rc) {
        return luaL_error(L, "link failed");
    }
    
    // TODO lelement(Lua userdata)如何保存连接的element的引用？
#if 0
    // 2 ref 1
    lua_getfenv(L, -1);
    //lua_traverse_table(L, -2);
    lua_pushstring(L, "pad1"); // k
    lua_pushvalue(L, -4); //v
    lua_rawset(L, -3);
    //lua_traverse_table(L, -2);
    lua_pop(L, 1);

    // 1 ref 2
    lua_getfenv(L, -2);
    assert(LUA_TTABLE == lua_type(L, -1));
    //rc = lua_gettop(L);
    //lua_traverse_table(L, -2);
    //rc = lua_gettop(L);
    lua_pushstring(L, "pad2"); // k
    //rc = lua_gettop(L);
    lua_pushvalue(L, -3); //v
    //lua_pushstring(L, "xx"); // v
    //lua_pushinteger(L, 33);

    //rc = lua_gettop(L);
    //lua_settable(L, -3);
    lua_rawset(L, -3);
    //rc = lua_gettop(L);
    //lua_traverse_table(L, -2);
    //rc = lua_gettop(L);
    lua_pop(L, 1);
    //rc = lua_gettop(L);
#else
    lua_pushvalue(L, elmidx1);
    rc = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pushvalue(L, elmidx2);
    rc = luaL_ref(L, LUA_REGISTRYINDEX);
#endif

    //lua_pushvalue(L, 1);
    return 0;
}

static int
on(lua_State *L)
{
    jlelement_t *self;
    const char *evt;
    jlelement_event_t *e;

    self = lua_touserdata(L, 1);
    evt = luaL_checkstring(L, 2);
    e = find_event(self, evt);
    if (lua_isfunction(L, 3)) {
        if (!e) {
            e = jarray_push(&self->events);
            e->evt = strdup(evt);
        }
        else {
            luaL_unref(L, LUA_REGISTRYINDEX, e->ref);
        }
        e->ref = luaL_ref(L, LUA_REGISTRYINDEX);
    }
    else if (lua_isnil(L, 3)) {
        if (e) {
            luaL_unref(L, LUA_REGISTRYINDEX, e->ref);
            e->ref = LUA_NOREF;
        }
    }
    else {
        return luaL_error(L, "function or nil expected");
    }
    return 0;
}

static int
inst_init(jlelement_t *self, va_list args)
{
    lua_State *L;
    jelement_class_t *cls;
    jelement_t *elm;
    jobject_class_t *objc, *pobjc;
    int rc;

    //L = va_arg(args, lua_State *);
    //cls = va_arg(args, jelement_class_t *);
    L = jlua_ctx()->L;
    cls = self->jet_inst_class;

    objc = (jobject_class_t *)cls;
    elm = malloc(objc->inst_size);
    rc = objc->constructor(elm, NULL);
    self->jet_inst = elm;
    elm->be_object = (jobject_t *)self;

    //pobjc = ((jobject_class_t *)(((jobject_t *)self)->_class))->parent_class;
    //jobject_class_t *c = ((jobject_t *)self)->_class;
    //pobjc = c->parent_class;

    // TODO set to _G, don't create
    lua_createtable(L, 0, 0);
    rc = lua_setfenv(L, -2);
    assert(1 == rc);
    rc = EOK;

    jarray_init(&self->events, 1, sizeof(jlelement_event_t));
    self->async_state = 0;

    return rc;
}

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


static void
vtbl_init(jlelement_class_t *self)
{
    jelement_class_t *elm;
    jobject_class_t *obj;
    jobject_class_t *pobj;
    int rc;
    //const struct luaL_Reg *l;
    //jlua_ctx_t *ctx;
    lua_State *L;
    luaL_Reg lib[] = {
        { "link", link },
        { "on", on },
        { NULL, NULL }
    };
    obj = (jobject_class_t *)self;

    ////props[0].method.type = "int (*)(jtcp_t *self, int backlog)";
    ////props[0].method.entry = jtcp_listen;
    //obj->properties = props;
    //obj->nproperties = ARRAY_SIZE(props);

    //elm = (jelement_class_t *)self;
    //elm->inst_size = sizeof(jtcp_t);
    //elm->name = "tcp";


    elm = (jelement_class_t *)self;
    //ctx = ;
    L = jlua_ctx()->L;

    // TODO use luaL_newmetatable luaL_checkudata

    // create meta table
    luaL_newmetatable(L, obj->tname);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__newindex");
    luaL_register(L, NULL, lib);
    //lua_rawset(L, LUA_REGISTRYINDEX);

    // set meta table's meta table
    pobj = obj->parent_class;
    //lua_pushlightuserdata(L, elm);
    //lua_rawget(L, LUA_REGISTRYINDEX);
    //lua_pushlightuserdata(L, pkls);
    //lua_rawget(L, LUA_REGISTRYINDEX);
    luaL_getmetatable(L, pobj->tname);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
}
#endif

