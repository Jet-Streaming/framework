#include "lrtmp.h"
#include <jet/packet.h>
#include <lua/jlthread.h>
#include "../amf.h"
#include "../message.h"

#define AMF_NULL        ((void *)0x4e554c4c) /* NULL */
#define AMF_UNDEFINED   ((void *)0x55444546) /* UDEF */

namespace jet {
namespace rtmp {

using namespace lua;

static const char *klass = "lrtmp";

lrtmp::lrtmp()
{
}

lrtmp::~lrtmp()
{
}

static int
__gc(lua_State *L)
{
    return 0;
}

static int
__tostring(lua_State *L)
{
    return 0;
}


//static int
//__newindex(lua_State *L)
//{
//    int rc;
//    jlrtmp_t *self;
//    const char *key;
//
//    //rc = lua_gettop(L);
//    //rc = LUA_TBOOLEAN;
//    //rc = lua_type(L, 3); // v
//    //rc = lua_type(L, 2); // k
//    //rc = lua_type(L, 1); // table
//
//    self = luaL_checkudata(L, 1, ((jobject_class_t *)&jlrtmp_class)->tname);
//    key = luaL_checklstring(L, 2, NULL);
//
//    //if (!strcmp(key, "on_request")) {
//    //    luaL_checktype(L, 3, LUA_TFUNCTION);
//    //    luaL_unref(L, LUA_REGISTRYINDEX, self->on_request);
//    //    lua_pushvalue(L, 3);
//    //    self->on_request = luaL_ref(L, LUA_REGISTRYINDEX);
//    //} else {
//    lua_getmetatable(L, 1);
//    lua_settable(L, -1);
//    //}
//
//    return 0;
//}

//static int
//amf_write_item(lua_State *L, jrtmp_t *self, jmbuf_t **mbuf, rtmp_amf_writer_t *ctx);
//
//static int
//amf_write_object(lua_State *L, jrtmp_t *self, jmbuf_t **mbuf, rtmp_amf_writer_t *ctx)
//{
//    int rc;
//    size_t l;
//    const char *str;
//
//    rc = rtmp_amf_write_obj(self, mbuf, ctx, 0);
//
//    lua_pushnil(L);  /* first key */
//    while (lua_next(L, -2) != 0) {
//        /* uses 'key' (at index -2) and 'value' (at index -1) */
//        if (LUA_TSTRING == lua_type(L, -2)) {
//            str = luaL_checklstring(L, -2, &l);
//            rc = rtmp_amf_write_string(self, mbuf, ctx, str, l);
//            rc = amf_write_item(L, self, mbuf, ctx);
//        }
//        else {
//            //printf("\t%s", lua_typename(L, lua_type(L, -2)));
//        }
//        lua_pop(L, 1);
//    }
//
//    rc = rtmp_amf_write_obj(self, mbuf, ctx, 1);
//}
//
//static int
//amf_write_item(lua_State *L, jrtmp_t *self, jmbuf_t **mbuf, rtmp_amf_writer_t *ctx)
//{
//    int rc;
//    size_t l;
//    const char *str;
//
//    switch (lua_type(L, -1)) {
//    case LUA_TSTRING:
//        str = luaL_checklstring(L, -1, &l);
//        rc = rtmp_amf_write_string(self, mbuf, ctx, str, l);
//        break;
//    case LUA_TTABLE:
//        rc = amf_write_object(L, self, mbuf, ctx);
//        break;
//    default:
//        rc = -EINVAL;
//    }
//    return rc;
//}
//
//static int
//amf_write_array(lua_State *L, jrtmp_t *self, jmbuf_t **mbuf, rtmp_amf_writer_t *ctx)
//{
//    int i,rc;
//
//    for (i = 1;; ++i) {
//        lua_pushinteger(L, i);
//        lua_rawget(L, -2);
//        if (LUA_TNIL == lua_type(L, -1)) {
//            lua_pop(L, 1);
//            return EOK;
//        }
//        rc = amf_write_item(L, self, mbuf, ctx);
//        lua_pop(L, 1);
//        if (rc) {
//            break;
//        }
//    }
//
//    return rc;
//}

static int
lnew(lua_State *L)
{
    void *t;

    t = lua_newuserdata(L, sizeof(lrtmp));
    new (t)lrtmp;

    luaL_getmetatable(L, klass);
    assert(lua_istable(L, -1));
    lua_setmetatable(L, -2);

    return 1;
}

static int
write_table(lua_State *L, int index, amf_writer *amfw);

static int
write_value(lua_State *L, int index, amf_writer *amfw, bool key = false)
{
    int rc;
    const char *str;
    size_t len;
    void *userdata;
    double db;
    int type = lua_type(L, index);

    switch (type) {
    case LUA_TNIL:
        rc = -ENODATA;
        break;
    case LUA_TBOOLEAN:
        rc = lua_toboolean(L, index);
        rc = amfw->boolean(!!rc);
        break;
    case LUA_TSTRING:
        str = lua_tolstring(L, index, &len);
        rc = amfw->string(str, len, key);
        break;
    case LUA_TNUMBER:
        db = lua_tonumber(L, index);
        rc = amfw->number(db);
        break;
    case LUA_TTABLE:
        rc = write_table(L, index, amfw);
        break;
    case LUA_TLIGHTUSERDATA:
        userdata = lua_touserdata(L, index);
        if (AMF_UNDEFINED == userdata) {
            rc = amfw->undefined();
        } else if (AMF_NULL == userdata) {
            rc = amfw->null();
        } else {
            rc = -EINVAL;
        }
        break;
    default:
        rc = -EINVAL;
    }
    return rc;
}

static int
write_map(lua_State *L, int index, amf_writer *amfw)
{
    int rc;

    rc = amfw->start_object();
    lua_pushnil(L);
    while (lua_next(L, index) != 0) {
        //int type = lua_type(L, -2);
        //case LUA_TSTRING:
        //    printf("\t$%s", lua_tolstring(L, -2, NULL));
        //    break;
        //case LUA_TNUMBER:
        //    printf("\t$%f", lua_tonumber(L, -2));
        //    break;
        //default:
        //    printf("\t%s", lua_typename(L, lua_type(L, -2)));
        //    break;
        //}
        //printf(" - ");
        //printf("%s\n", lua_typename(L, lua_type(L, -1)));
        int type = lua_type(L, index + 1);
        if (LUA_TSTRING == type) {
            rc = write_value(L, index + 1, amfw, true);
            rc = write_value(L, index + 2, amfw);
        } else {
            rc = -EINVAL;
        }
        lua_pop(L, 1);
    }
    return amfw->end_object();
}

static int
write_array(lua_State *L, int index, amf_writer *amfw)
{
    int rc;

    rc = amfw->start_array();
    for (int i = 1; EOK == rc; ++i) {
        lua_rawgeti(L, index, i);
        rc = write_value(L, index + 1, amfw);
        lua_pop(L, 1);
    }
    if (-ENODATA != rc) {
        return rc;
    }
    return amfw->end_array();
}

static int
write_table(lua_State *L, int index, amf_writer *amfw)
{
    // https://www.lua.org/pil/27.1.html
    assert(index > 0);
    lua_rawgeti(L, index, 1);
    int type = lua_type(L, -1);
    lua_pop(L, 1);

    int rc;
    if (LUA_TNIL == type) {
        rc = write_map(L, index, amfw);
    } else {
        rc = write_array(L, index, amfw);
    }

    return rc;
}

int
lrtmp::lcall(lua_State *L)
{
    int rc;
    lrtmp *self = (lrtmp *)luaL_checkudata(L, 1, klass);
    int msid = luaL_checkint(L, 2);
    luaL_checktype(L, 4, LUA_TTABLE);
    int transaction = luaL_optint(L, 3, 0);

    //jlua::traverse_table(L, 3);

    if (transaction) {
        if (self->transactions.count(transaction)) {
            lua_pushinteger(L, -EBUSY);
            return 1;
        }
    }

    // TODO use shared_ptr
    rtmp_message *msg = new rtmp_message;
    rc = msg->alloc(self->chunk_size);
    if (rc) {
        lua_pushinteger(L, rc);
        return 1;
    }

    msg->type = RTMP_MSG_AMF_CMD;
    msg->timestamp = 0;
    msg->msid = msid;

    amf_writer amfw(msg, self->chunk_size);
    rc = write_table(L, 4, &amfw);
    if (rc) {
        lua_pushinteger(L, rc);
        return 1;
    }
    msg->mlen = msg->len();

    rc = self->message_pad.push(&self->message_pushout, msg);
    if (rc) {
        lua_pushinteger(L, rc);
        return 1;
    }

    thread *th = thread::to_thread(L);
    if (transaction) {
        self->transactions[transaction] = th;
    }
    return th->yield(0);
}

int
lrtmp::lset_chunk_size(lua_State *L)
{
    lrtmp *self = (lrtmp *)luaL_checkudata(L, 1, klass);
    int cs = luaL_checkint(L, 2);
    // TODO check cs
    self->chunk_size = cs;
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
        //{ "__newindex", __newindex },
        { "__gc", __gc },
        { "__tostring", __tostring },
        //{ "request", request },
        { "call", lrtmp::lcall },
        { "set_chunk_size", lrtmp::lset_chunk_size },
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


//static int
//on_begin(jrtmp_t *self)
//{
//    lua_State *L;
//    jlrtmp_t *lself;
//    int rc;
//
//    lself = self->parent.be_object;
//    L = jlua_ctx()->L;
//    lself->origin_top = lua_gettop(L);
//    lua_newtable(L);
//
//    return EOK;
//}
//
//static int
//on_end(jrtmp_t *self)
//{
//    lua_State *L;
//    int rc;
//    jlrtmp_t *lself;
//
//    L = jlua_ctx()->L;
//    lself = self->parent.be_object;
//
//    rc = lua_gettop(L);
//    rc = jlelement_push_listener(lself, "amf");
//    if (rc) {
//        return rc;
//    }
//    rc = lua_gettop(L);
//    lua_pushvalue(L, -2);
//
//    rc = lua_gettop(L);
//    rc = lua_pcall(L, 1, 0, 0);
//
//    rc = lua_gettop(L);
//    lua_settop(L, lself->origin_top);
//
//    return EOK;
//}
//
//static int
//on_number(jrtmp_t *self, double n)
//{
//    lua_State *L;
//    size_t size;
//
//    L = jlua_ctx()->L;
//    if (self->amf_object_depth) {
//        if (!self->amf_object_v) {
//            return -EINVAL;
//        }
//        lua_pushnumber(L, n);
//        lua_rawset(L, -3);
//    } else {
//        luaL_checktype(L, -1, LUA_TTABLE);
//        size = lua_objlen(L, -1);
//        lua_pushinteger(L, size + 1);
//        lua_pushnumber(L, n);
//        lua_rawset(L, -3);
//    }
//
//    return EOK;
//}
//
//static int 
//on_boolean(jrtmp_t *self, int n)
//{
//    return EOK;
//}
//
//static int 
//on_string(jrtmp_t *self, const char *s, uint32_t len)
//{
//    lua_State *L;
//    int rc;
//    jlrtmp_t *lself;
//    size_t size;
//
//    lself = self->parent.be_object;
//    if (!s) {
//        if (len > 1024) {
//            return -EINVAL;
//        }
//        lself->str = lself->pos = realloc(lself->str, len);
//        lself->strl = len;
//        return EOK;
//    }
//    else {
//        if (lself->pos + len > lself->str + lself->strl) {
//            return -EINVAL;
//        }
//        memcpy(lself->pos, s, len);
//        lself->pos += len;
//        if (lself->pos < lself->str + lself->strl) {
//            return EOK;
//        }
//    }
//
//    assert(lself->pos == lself->str + lself->strl);
//    L = jlua_ctx()->L;
//
//    if (self->amf_object_depth) {
//        lua_pushlstring(L, lself->str, lself->strl);
//        if (self->amf_object_v) {
//            lua_rawset(L, -3);
//        }
//    }
//    else {
//        luaL_checktype(L, -1, LUA_TTABLE);
//        size = lua_objlen(L, -1);
//        lua_pushinteger(L, size + 1);
//        lua_pushlstring(L, lself->str, lself->strl);
//        lua_rawset(L, -3);
//    }
//
//    return EOK;
//}
//
//static int
//on_object(jrtmp_t *self)
//{
//    lua_State *L;
//    //jlrtmp_t *lself;
//    int rc;
//
//    //lself = self->parent.be_object;
//    L = jlua_ctx()->L;
//    lua_newtable(L);
//
//    return EOK;
//}
//
//static int
//on_object_end(jrtmp_t *self)
//{
//    lua_State *L;
//    int rc;
//    size_t size;
//
//    L = jlua_ctx()->L;
//    if (self->amf_object_depth) {
//        lua_pushvalue(L, -2);
//        if (!self->amf_object_v) {
//            // object cannot a key
//            return -EINVAL;
//        }
//        lua_rawset(L, -3);
//    }
//    else {
//        luaL_checktype(L, -1, LUA_TTABLE);
//        luaL_checktype(L, -2, LUA_TTABLE);
//        size = lua_objlen(L, -2);
//        lua_pushinteger(L, size + 1);
//        lua_pushvalue(L, -2);
//        lua_rawset(L, -4);
//    }
//    lua_pop(L, 1);
//
//    return EOK;
//}
//
//
//static jrtmp_amf_callback_t
//amf_cb = {
//    .on_begin = on_begin,
//    .on_end = on_end,
//    .on_number = on_number,
//    .on_boolean = on_boolean,
//    .on_string = on_string,
//    .on_object = on_object,
//    .on_object_end = on_object_end,
//};

static int
linit(lua_State *L)
{
    int rc;

    lua_newtable(L);
    const luaL_Reg lib[] = {
        { "new", lnew },
        { NULL, NULL }
    };
    luaL_register(L, NULL, lib);
    lua_setfield(L, -2, "rtmp");

    lua_newtable(L);
    lua_pushlightuserdata(L, AMF_NULL);
    lua_setfield(L, -2, "null");
    lua_pushlightuserdata(L, AMF_UNDEFINED);
    lua_setfield(L, -2, "undefined");
    lua_setfield(L, -2, "amf");

    return linit_mt(L);
}

static initializer
li(linit);

JLUA_EXPORT int
luaopen_adobe_rtmp(lua_State *L)
{
    int rc;

    lua_newtable(L);
    rc = initializer::initialize(L);

    return 1;
}

} // rtmcsp
} // jet