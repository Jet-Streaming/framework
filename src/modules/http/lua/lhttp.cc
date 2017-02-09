#include "lhttp.h"
//#include "../tcp.h"
#include <lua/jlthread.h>
#include <jet/sequence.h>

namespace jet {
namespace http {

using namespace lua;

static const char *klass = "lhttp";

lhttp::lhttp()
    : http(&pss)
{
}

lhttp::~lhttp()
{
}

static int
lnew(lua_State *L)
{
    void *t;

    t = lua_newuserdata(L, sizeof(lhttp));
    new (t)lhttp;

    luaL_getmetatable(L, klass);
    assert(lua_istable(L, -1));
    lua_setmetatable(L, -2);

    return 1;
}

static int
lrequest(lua_State *L)
{
    lhttp *self;
    sequence *pkt;
    int rc;

    self = (lhttp *)luaL_checkudata(L, 1, klass);
    if (!lua_istable(L, 2)) {
        return luaL_error(L, "option expected");
    }

    if (self->waiting_thread) {
        lua_pushnil(L);
        lua_pushliteral(L, "busy element");
        return 2;
    }

    pkt = new sequence;
    rc = pkt->alloc();
    self->gen_request_line(pkt, HTTP_GET, "/", 1, 1, 1);
    self->gen_header(pkt, "Host", 4, "www.baidu.com", 13);
    self->gen_header(pkt, "Accept", 6, "*/*", 3);
    self->gen_headers_done(pkt);

    rc = self->request(pkt, self);

    thread *th = thread::to_thread(L);
    self->waiting_thread = th;
    return th->yield(0);
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
        { "request", lrequest },
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
    lua_setfield(L, -2, "http");

    rc = linit_mt(L);

    return rc;
}

static initializer
li(linit);


//#include "lhttp.h"
//#include "../http.h"
//
//#include <jet/buffer.h>
//#include <jet/string.h>
//#include <jlthread.h>
//
////static const char *req_key = "lhttp.req";
//
//enum {
//    ASYNC_RESPONSE = 1,
//};
//
//typedef enum {
//    LHS_INVALID = 0,
//    LHS_URI,
//    LHS_HEADER_FIELD,
//    LHS_HEADER_VALUE,
//} lhttp_state_t;
//


#define h_major_version "major_version"
#define h_minor_version "minor_version"
#define h_epoch  "epoch"
#define h_uri    "uri"
#define t_header "header"
#define h_status "status"

//
//static void l_message(const char *pname, const char *msg) {
//    if (pname) fprintf(stderr, "%s: ", pname);
//    fprintf(stderr, "%s\n", msg);
//    fflush(stderr);
//}
//
//
//static int report(lua_State *L, int status) {
//    if (status && !lua_isnil(L, -1)) {
//        const char *msg = lua_tostring(L, -1);
//        if (msg == NULL) msg = "(error object is not a string)";
//        l_message(NULL, msg);
//        lua_pop(L, 1);
//    }
//    return status;
//}
//
//
////static int
////print(lua_State *L)
////{
////    return 0;
////}
//

// https://github.com/openresty/lua-nginx-module#ngxlocationcapture

int
lhttp::on_message_begin(http_parser *p)
{
    //lhttp_req_t *req;
    //int thread;

    lhttp *self = static_cast<lhttp *>(p->data);
    thread *th = self->waiting_thread;
	assert(th); // TODO thread may be killed
    lua_State *L = th->from_thread();

    ////req = jarray_push(&self->reqs);
    //
    ////co = lua_newthread(L);
    ////lua_rawgeti(co, LUA_REGISTRYINDEX, self->on_request);
    lua_newtable(L); // req/resp
    ////req = lua_newuserdata(co, sizeof(lhttp_req_t));
    ////luaL_getmetatable(co, req_key);
    ////lua_setmetatable(co, -2);

    //lua_pushliteral(L, h_epoch);
    //lua_pushinteger(L, uv_now(uv_default_loop()));
    //lua_rawset(L, -3);

    //req = malloc(sizeof(lhttp_req_t));
    //req->http = self;
    ////req->buflen = 0;
    ////req->buf2len = 0;
    ////req->urilen = 0;
    ////req->thread = luaL_ref(L, LUA_REGISTRYINDEX);
    //req->table = luaL_ref(L, LUA_REGISTRYINDEX);
    //req->state = LHS_INVALID;

    //p->data = req;

    return 0;
}

//static int
//save_buf(lhttp_buf_t *buf, const char *at, size_t length)
//{
//    if (length > sizeof(buf->b) - buf->l) {
//        return -1;
//    }
//
//    memcpy(buf->b + buf->l, at, length);
//    buf->l += length;
//    assert(buf->l <= sizeof(buf->b));
//
//    return 0;
//}
//// TODO int on_uri(http_parser *p, const char *at, size_t length, int ended/* 0 or 1 */);
int
lhttp::on_url(http_parser *p, const char *at, size_t length)
{
	lhttp *self;

	self = static_cast<lhttp *>(p->data);
	self->parser_v.append(at, length);
    return 0;
}


////// multi enter
////static int
////on_data(http_parser *p, const char *at, size_t length)
////{
////    lhttp_req_t *req;
////
////    req = p->data;
////
////    if (length > sizeof(req->buf) - req->buflen) {
////        return -1;
////    }
////
////    memcpy(req->buf + req->buflen, at, length);
////    req->buflen += length;
////    assert(req->buflen <= sizeof(req->buf));
////
////    return 0;
////}
////
////static int
////on_data2(http_parser *p, const char *at, size_t length)
////{
////    lhttp_req_t *req;
////
////    req = p->data;
////
////    if (length > sizeof(req->buf2) - req->buf2len) {
////        return -1;
////    }
////
////    memcpy(req->buf2 + req->buf2len, at, length);
////    req->buf2len += length;
////    assert(req->buf2len <= sizeof(req->buf2));
////
////    return 0;
////}
//
//static void
//push_buf(lhttp_req_t *req, lhttp_buf_t *k, lhttp_buf_t *v)
//{
//    lua_State *L;
//
//    L = jlua_ctx()->L;
//    lua_rawgeti(L, LUA_REGISTRYINDEX, req->table);
//    lua_pushlstring(L, k->b, k->l);
//    lua_pushlstring(L, v->b, v->l);
//    lua_rawset(L, -3);
//    lua_pop(L, 1);
//}
//
int
lhttp::on_url_complete(http_parser *p)
{
    //lhttp_req_t *req;
	lhttp *self;
	thread *ctx;
	lua_State *L;

	assert(0);
	self = static_cast<lhttp *>(p->data);
	//L = 
    //req = p->data;
    //assert(LHS_URI == req->state);

    //strcpy(req->k.b, h_uri);
    //req->k.l = 3;
    //push_buf(req, &req->k, &req->v);

	//lua_pushliteral(L, "uri");
	//lua_pushlstring(L, self->);
	//lua_rawset(L, -3);
	self->parser_v.clear();

    return 0;
}

int
lhttp::on_status(http_parser *p, const char *at, size_t length)
{
    lhttp *self;

    self = static_cast<lhttp *>(p->data);
    //self->parser_v.append(at, length);
    return 0;
}

int
lhttp::on_status_complete(http_parser *p)
{
    lhttp *self;

    self = static_cast<lhttp *>(p->data);
    // TODO push string
    //self->parser_v.clear();
    return 0;
}

int
lhttp::on_header_field(http_parser *p, const char *at, size_t length)
{
	lhttp *self;

	self = static_cast<lhttp *>(p->data);
	self->parser_k.append(at, length);
	return 0;
}

int
lhttp::on_header_value(http_parser *p, const char *at, size_t length)
{
	lhttp *self;

	self = static_cast<lhttp *>(p->data);
	self->parser_v.append(at, length);
	return 0;
}

int
lhttp::on_header_complete(http_parser *p)
{
	lhttp *self = static_cast<lhttp *>(p->data);
	thread *th = self->waiting_thread;
	assert(th); // TODO thread may be killed
	lua_State *L = th->from_thread();
    //req = p->data;
    //header = "in_header";

    //lua_rawgeti(L, LUA_REGISTRYINDEX, req->table);
    lua_getfield(L, -1, t_header);
    if (LUA_TNIL == lua_type(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_pushliteral(L, t_header); // req.header
        lua_pushvalue(L, -2);
        lua_rawset(L, -4);
    }

    ////if (LUA_NOREF == req->req_header_table) {
    ////    lua_newtable(co);
    ////    lua_pushvalue(co, -1);
    ////    req->req_header_table = luaL_ref(co, -1);
    ////}

    ////lua_pushlstring(co, req->buf, req->buflen);
    ////lua_pushlstring(co, req->buf2, req->buf2len);
    ////req->buflen = 0;
    ////req->buf2len = 0;
    ////lua_rawset(co, -3);

    lua_pushlstring(L, self->parser_k.data(), self->parser_k.length());
	lua_pushlstring(L, self->parser_v.data(), self->parser_v.length());
	lua_rawset(L, -3);
    lua_pop(L, 1); // req.header

	self->parser_k.clear();
	self->parser_v.clear();

    return 0;
}

static int
on_headers_complete(http_parser *p)
{
    int rc;
    lhttp *self = static_cast<lhttp *>(p->data);
    thread *th = self->waiting_thread;
    assert(th); // TODO thread may be killed
    lua_State *L = th->from_thread();

    //lua_rawgeti(L, LUA_REGISTRYINDEX, self->on_request);
    //jlelement_push_listener(self, "request");
    //lua_rawgeti(L, LUA_REGISTRYINDEX, req->table);
    
    lua_pushliteral(L, h_major_version);
    lua_pushnumber(L, p->http_major);
    lua_rawset(L, -3);

    lua_pushliteral(L, h_minor_version);
    lua_pushnumber(L, p->http_minor);
    lua_rawset(L, -3);

    // status_code
    lua_pushliteral(L, h_status);
    lua_pushnumber(L, p->status_code);
    lua_rawset(L, -3);

    // method
    //lua_pushliteral(L, "method");
    //lua_pushinteger(L, p->method);
    //lua_rawset(L, -3);



    //// free req


    ////lua_newtable(co); // res
    ////rc = lua_resume(co, 1);
    //rc = lua_pcall(L, 1, 0, 0);
    ////report(L, rc);

    ////rc = jlthread_pcall(L, 1, 0);
    //if (rc) {
    //    return -1;
    //}

    self->waiting_thread = nullptr;
    rc = th->resume(1);
    if (rc) {
        ;
    }

    return EOK;
}

static int
on_message_complete(http_parser *p)
{
    return 0;
}

http_parser_settings 
lhttp::pss = {
    on_message_begin,
    on_url,
    on_url_complete,
    on_status,
    on_status_complete,
    on_header_field,
    on_header_value,
    on_header_complete,
    on_headers_complete,
    nullptr,
    on_message_complete,
    nullptr, // on_chunk_header
    nullptr, // on_chunk_complete
	nullptr, // on_error
};
//
//static int
//request(lua_State *L)
//{
//    return 0;
//}
//
//
//
//static int
//gen_response(lua_State *L, int req, jmbuf_t **mbuf)
//{
//    int status, rc;
//    //jmbuf_t *lastmb;
//    const char *status_line, *k, *v;
//    size_t ks, vs;
//
//    lua_pushvalue(L, req);
//
//    lua_pushliteral(L, h_status);
//    lua_rawget(L, -2);
//    status = luaL_optint(L, -1, 0);
//    lua_pop(L, 1);
//
//    lua_pushliteral(L, "reason");
//    lua_rawget(L, -2);
//    status_line = luaL_optstring(L, -1, NULL);
//    lua_pop(L, 1);
//
//    rc = jhttp_resp_status(mbuf, status, status_line);
//
//    // out_headers
//    lua_pushliteral(L, "out_header");
//    lua_rawget(L, -2);
//    if (lua_istable(L, -1)) {
//        lua_pushnil(L);  /* first key */
//        while (lua_next(L, -2) != 0) {
//            /* uses 'key' (at index -2) and 'value' (at index -1) */
//            //if (LUA_TSTRING == lua_type(L, -2)) {
//            //    printf("\t$%s", lua_tolstring(L, -2, NULL));
//            //}
//            //else {
//            //    printf("\t%s", lua_typename(L, lua_type(L, -2)));
//            //}
//            //printf(" - ");
//            //printf("%s\n", lua_typename(L, lua_type(L, -1)));
//
//            k = lua_tolstring(L, -2, &ks);
//            v = lua_tolstring(L, -1, &vs);
//            rc = jhttp_resp_header(mbuf, k, v);
//
//            /* removes 'value'; keeps 'key' for next iteration */
//            lua_pop(L, 1);
//        }
//    }
//    lua_pop(L, 1);
//    rc = jhttp_resp_header(mbuf, NULL, NULL);
//
//    lua_pop(L, 1); // req
//
//    // body : 只接受body_pad的数据
//    //v = lua_tolstring(L, req + 1, &vs);
//    //if (v) {
//    //    jmbuf_memcpy(mbuf, &lastmb, v, vs);
//    //}
//
//    return 0;
//}
//
//static void
//response_cleanup(void *p)
//{
//    jhttp_t *http;
//
//    http = p;
//    //jpad_cancel_push();
//    //jpad_unref_push(http->http_out_push);
//    http->http_out_push = NULL;
//}
//
////struct response_ctx {
////    lua_State *co;
////};
//
//static void
//response_cb(jpad_push_t *push)
//{
//    jhttp_t *http;
//    lua_State *L;
//    int rc;
//
//    http = container_of(push->sender, jhttp_t, http_pad);
//    //assert(ASYNC_RESPONSE == ((jlelement_t *)http)->async_state);
//    //((jlelement_t *)http)->async_state = 0;
//    //L = luaJ_thread_get(http);
//    L = http->userdata;
//    lua_pushinteger(L, push->status);
//    //rc = lua_resume(L, 1);
//    rc = jlthread_resume(L, 1);
//}
//
//static int
//response(lua_State *L)
//{
//    jlhttp_t *self;
//    jmbuf_t *mbuf;
//    jhttp_t *http;
//    int rc, out_sent;
//    const char *errstr;
//    //struct response_ctx *rctx;
//
//    self = luaL_checkudata(L, 1, ((jobject_class_t *)jlhttp_class())->tname);
//    luaL_checktype(L, 2, LUA_TTABLE); // req
//
//    mbuf = NULL;
//    errstr = "unknown error";
//
//    lua_pushliteral(L, "out_sent");
//    lua_rawget(L, 2);
//    out_sent = luaL_optint(L, -1, 0);
//    if (out_sent) {
//        errstr = "duplicate response";
//        goto err;
//    }
//
//    http = ((jlelement_t *)self)->jet_inst;
//    mbuf = jmbuf_new(NULL, &http->http_pad);
//    rc = gen_response(L, 2, &mbuf);
//
//    http->http_out_push = jpad_new_push(response_cb, 0);
//    //rctx = (int8_t *)http->http_out_push + sizeof(struct response_ctx);
//    //rctx->co = L;
//    http->userdata = L;
//    rc = jpad_push(&http->http_pad, http->http_out_push, mbuf);
//    assert(!((jlelement_t *)self)->async_state);
//    ((jlelement_t *)self)->async_state = ASYNC_RESPONSE;
//
//    jlthread_set_cleanup(L, response_cleanup, http);
//
//    // 应该用req的指针，http有可能有多个并行的req
//    return jlthread_yield(L, http, 0);
//
//err:
//    jmbuf_free(mbuf);
//    lua_pushnil(L);
//    lua_pushstring(L, errstr);
//    return 2;
//}
//
//static int
//_gc(lua_State *L)
//{
//    jhttp_t *self = lua_touserdata(L, 1);
//    memset(self, 0xdeadbeef, sizeof(jhttp_t));
//    return 0;
//}
//
//static int
//__tostring(lua_State *L)
//{
//    return 0;
//}
//
//static int
//__newindex(lua_State *L)
//{
//    int rc;
//    jlhttp_t *self;
//    const char *key;
//
//    //rc = lua_gettop(L);
//    //rc = LUA_TBOOLEAN;
//    //rc = lua_type(L, 3); // v
//    //rc = lua_type(L, 2); // k
//    //rc = lua_type(L, 1); // table
//
//    self = luaL_checkudata(L, 1, ((jobject_class_t *)&jlhttp_class)->tname);
//    key = luaL_checklstring(L, 2, NULL);
//
//    //if (!strcmp(key, "on_request")) {
//    //    luaL_checktype(L, 3, LUA_TFUNCTION);
//    //    luaL_unref(L, LUA_REGISTRYINDEX, self->on_request);
//    //    lua_pushvalue(L, 3);
//    //    self->on_request = luaL_ref(L, LUA_REGISTRYINDEX);
//    //} else {
//        lua_getmetatable(L, 1);
//        lua_settable(L, -1);
//    //}
//
//    return 0;
//}
//
//static const luaL_Reg lib[] = {
//    { "__newindex", __newindex },
//    { "__gc", _gc },
//    //{ "__tostring", __tostring },
//    { "request", request },
//    { "response", response },
//    { NULL, NULL }
//};
//
////static const luaL_Reg req_mt[] = {
////    { "print", print },
////    //{ "say", say },
////    { NULL, NULL }
////};
//

JLUA_EXPORT int
luaopen_http(lua_State *L)
{
    int rc;

    lua_newtable(L);
    rc = initializer::initialize(L);

    return 1;
}

} // http
} // jet

//static int
//inst_init(jlhttp_t *self, va_list args)
//{
//    jelement_t *elm;
//    jhttp_t *http;
//    int rc;
//    lua_State *L;
//    jobject_class_t *obj;
//
//    //elm = (jelement_t *)self;
//
//    http = ((jlelement_t *)self)->jet_inst;
//    http->parser.data = self;
//    http->setting = &pss;
//
//    //self->on_request = LUA_NOREF;
//
//    //jarray_init(&self->reqs, 1, sizeof(lhttp_req_t *));
//
//    obj = (jobject_class_t *)&jlhttp_class;
//    //L = va_arg(args, lua_State *);
//    L = jlua_ctx()->L;
//    luaL_getmetatable(L, obj->tname);
//    lua_setmetatable(L, -2);
//
//    //// TODO set to _G, don't create
//    //lua_createtable(L, 0, 0);
//    //rc = lua_setfenv(L, -2);
//    //assert(1 == rc);
//
//    return EOK;
//}
//
//static void
//vtbl_init(jlhttp_class_t *self)
//{
//    jlelement_class_t *elm;
//    jobject_class_t *obj, *pobj;
//    int rc;
//    //const struct luaL_Reg *l;
//    //jlua_ctx_t *ctx;
//    lua_State *L;
//
//    obj = (jobject_class_t *)self;
//
//    ////props[0].method.type = "int (*)(jtcp_t *self, int backlog)";
//    ////props[0].method.entry = jtcp_listen;
//    //obj->properties = props;
//    //obj->nproperties = ARRAY_SIZE(props);
//
//    elm = (jlelement_class_t *)self;
//    elm->inst_size = sizeof(jlhttp_t);
//    elm->name = "http";
//
//    L = jlua_ctx()->L;
//
//    // create meta table
//    luaL_newmetatable(L, obj->tname);
//    lua_pushvalue(L, -1);
//    lua_setfield(L, -2, "__index");
//    luaL_register(L, NULL, lib);
//
//    // set meta table's meta table
//    pobj = obj->parent_class;
//    luaL_getmetatable(L, pobj->tname);
//    lua_setmetatable(L, -2);
//    lua_pop(L, 1);
//
//    //luaL_newmetatable(L, req_key);
//    //lua_pushvalue(L, -1);
//    //lua_setfield(L, -2, "__index");
//    //luaL_register(L, NULL, req_mt);
//    //lua_pop(L, 1);
//}
//
//J_DEFINE_CLASS(jlhttp, jlelement, inst_init, vtbl_init);