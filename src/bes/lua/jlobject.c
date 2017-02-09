#include "jlobject.h"



//static const luaL_Reg
//lib[] = {
//    { "link", link },
//    { NULL, NULL }
//};

static int
inst_init(jlobject_t *self, va_list args)
{
    return EOK;
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

//static int
//to_string(jobject_t *self, char *buf, size_t len)
//{
//    return snprintf(buf, len, "%p", self);
//}


static int
__index(lua_State *L)
{
    // t, k
    return luaL_error(L, "no index");
}

static int
__newindex(lua_State *L)
{
    // t, k, v
    return luaL_error(L, "no newindex");
}

static const luaL_Reg
lib[] = {
    { "__newindex", __newindex },
    { "__index", __index },
    { NULL, NULL }
};
static void
vtbl_init(jlobject_class_t *self)
{
    //jelement_class_t *elm;
    jobject_class_t *obj;
    //jobject_class_t *pobj;
    int rc;
    //const struct luaL_Reg *l;
    //jlua_ctx_t *ctx;
    lua_State *L;

    obj = (jobject_class_t *)self;

    ////props[0].method.type = "int (*)(jtcp_t *self, int backlog)";
    ////props[0].method.entry = jtcp_listen;
    //obj->properties = props;
    //obj->nproperties = ARRAY_SIZE(props);

    //elm = (jelement_class_t *)self;
    //elm->inst_size = sizeof(jtcp_t);
    //elm->name = "tcp";


    //elm = (jelement_class_t *)self;
    L = jlua_ctx()->L;

    // create meta table
    //pobj = obj->parent_class;
    luaL_newmetatable(L, obj->tname);
    luaL_register(L, NULL, lib);
    lua_pop(L, 1);
}

J_DEFINE_CLASS(jlobject, jobject, inst_init, vtbl_init);