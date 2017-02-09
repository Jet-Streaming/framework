#include "jlthread.h"

//#define fromstate(l)	((char *)(l) - LUAI_EXTRASPACE)
//#define tostate(l)   (cast(lua_State *, cast(lu_byte *, l) + LUAI_EXTRASPACE))

namespace jet {
namespace lua {

//struct thread_priv {
//    thread *ctx;
//};

extern "C" unsigned 
LUAI_EXTRASPACE = sizeof(thread);
static lua_CFunction 
coro_create = nullptr;
static lua_CFunction 
coro_resume = nullptr;
static lua_CFunction 
coro_yield = nullptr;

//static thread *
//from_thread(lua_State *L)
//{
//    char *c = (char *)L - LUAI_EXTRASPACE;
//    return (thread *)c;
//}

thread *
thread::to_thread(lua_State *L)
{
    char *c = (char *)L - LUAI_EXTRASPACE;
    return (thread *)c;
}

static int
lspawn(lua_State *L)
{
    luaL_argcheck(L, lua_isfunction(L, 1) && !lua_iscfunction(L, 1), 1,
        "Lua function expected");

    lua_State *NL = lua_newthread(L);
    ////lua_gc(L, LUA_GCCOLLECT, 0);

    int rc = lua_gettop(L);
    //for (i = 1; i != rc; ++i) {
    //    //lua_pushvalue(L, i);
    //    lua_xmove(L, NL, 1);
    //}
    lua_xmove(L, NL, rc);

    //priv = fromstate(NL);
    //priv->ctx = new thread(nl, NL, 1);

    thread *nth = thread::to_thread(NL);
    nth->thrd = 1;
    rc = nth->resume(rc - 1);
    lua_rawgeti(L, LUA_REGISTRYINDEX, nth->ref);
    //luaL_unref(L, LUA_REGISTRYINDEX, nl);

    return 1;
}

static int
lwait(lua_State *L)
{
    assert(0);
    return 0;
}

//static int
//lcreate(lua_State *L)
//{
//    int rc;
//    lua_State *NL;
//    thread_priv *priv;
//
//    rc = coro_create(L);
//    if (1 != rc) {
//        return rc;
//    }
//
//    NL = lua_tothread(L, -1);
//    priv = fromstate(NL);
//    int nl = luaL_ref(L, LUA_REGISTRYINDEX);
//    priv->ctx = new thread(nl, NL, 0);
//
//    return rc;
//}

static int
lresume(lua_State *L)
{
    lua_State *co = lua_tothread(L, 1);

    luaL_argcheck(L, co, 1, "coroutine expected");
    thread *t = thread::to_thread(co);
    luaL_argcheck(L, !t->thrd, 1, "coroutine expected");
    return coro_resume(L);
}

static int
lyield(lua_State *L)
{
    // TODO thread can yield
    thread *t = thread::to_thread(L);
    luaL_argcheck(L, !t->thrd, 1, "coroutine expected");
    return coro_yield(L);
}

static void
stat_cs(void *_L)
{
    lua_State *L = (lua_State *)_L;
    thread *t = thread::to_thread(L);
    ++t->csid;
}


extern "C" void
luai_userstateopen(void *_L)
{
    lua_State *L = static_cast<lua_State *>(_L);
    thread *th = thread::to_thread(L);
    new (th)thread;
}

extern "C" void
luai_userstateclose(void *_L)
{
    assert(0);
    // unref
    // call cleanup
}

extern "C" void
luai_userstatethread(void *_L, void *_L1)
{
    lua_State *L = (lua_State *)_L;
    lua_State *NL = (lua_State *)_L1;
    thread *nth = thread::to_thread(NL);
    new (nth)thread;
    nth->ref = luaL_ref(L, LUA_REGISTRYINDEX);

    thread *th = thread::to_thread(L);
    lelement *elm = th->get_element();
    nth->set_element(elm);
}

extern "C" void
luai_userstatefree(void *_L)
{
    assert(0);
    // unref
    // call cleanup
}

extern "C" void
luai_userstateresume(void *_L, int nargs)
{
    stat_cs(_L);
}

extern "C" void
luai_userstateyield(void *_L, int nargs)
{
    stat_cs(_L);
}

/////////////////////////

lua_State *
thread::from_thread()
{
    char *c = (char *)this + LUAI_EXTRASPACE;
    return (lua_State *)c;
}

static const char *
element_key = "__jet_elm";

void 
thread::set_element(lelement *e)
{
    if (!e) {
        return;
    }
    lua_State *L = from_thread();
    lua_pushlightuserdata(L, e);
    lua_setglobal(L, element_key);
}

lelement *
thread::get_element()
{
    lua_State *L = from_thread();
    lua_getglobal(L, element_key);
    void *e = (lua_touserdata(L, -1));
    lua_pop(L, 1);

    return static_cast<lelement *>(e);
}


void
thread::set_cleanup(lua_State *L, cleanup_cb cb, void *userdata)
{
    thread *t = thread::to_thread(L);
    t->cleanup = cb;
    t->cleanup_userdata = userdata;
}

//uint32_t
//thread::csid(lua_State *L)
//{
//    thread_priv *priv;
//
//    priv = fromstate(L);
//    return priv->csid;
//}


/* 上层如何流控：
    1.返回EINPROGRES，framework就先不会ack；
    2.上层调用API暂停流的接收（类似disable UI控件），后面再恢复（使能）
*/
int
thread::pcall(lua_State *L, int nargs, int nresults)
{
    int rc;

    rc = lua_pcall(L, nargs, nresults, 0);
    rc = jlua::report(L, rc);
    return rc;
}

int
thread::resume(int narg)
{
    lua_State *L = from_thread();
    int rc = lua_resume(L, narg);
    if (LUA_YIELD == rc) {
        rc = 0;
    } else if (0 == rc) {
        // finishes its execution without errors
        // delete this
    } else {
        // error occur, stack is not unwound
        rc = jlua::report(L, rc);
        // delete this
    }
    return rc;
}

int
thread::yield(int nresults)
{
    lua_State *L = from_thread();
    //luaJ_thread_set(L, ctx);
    return lua_yield(L, nresults);
}

// { ltcp_t *, coroutine(weak) }
//int
//luaJ_thread_set(lua_State *co, void *k)
//{
//    jlua_ctx_t *ctx;
//    //lua_State *L;
//    int rc;
//
//    ctx = jlua_ctx();
//    //L = ctx->L;
//
//    lua_rawgeti(co, LUA_REGISTRYINDEX, ctx->waiting_corotines);
//    lua_pushlightuserdata(co, k);
//    rc = lua_pushthread(co);
//    lua_rawset(co, -3);
//    lua_pop(co, 1);
//
//    // main thread cannot yield
//    assert(!rc);
//
//    return rc;
//}
//
//lua_State *
//luaJ_thread_get(void *k)
//{
//    jlua_ctx_t *ctx;
//    lua_State *L, *co;
//
//    ctx = jlua_ctx();
//    L = ctx->L;
//
//    lua_rawgeti(L, LUA_REGISTRYINDEX, ctx->waiting_corotines);
//    lua_pushlightuserdata(L, k);
//    lua_rawget(L, -2);
//    co = lua_tothread(L, -1);
//    lua_pop(L, 1);
//    return co;
//}

static void
hook(lua_State *L, const char *fn, lua_CFunction *old, lua_CFunction _new)
{
    lua_pushstring(L, fn);
    lua_pushvalue(L, -1);
    lua_rawget(L, -3);
    assert(!*old);
    *old = lua_tocfunction(L, -1);
    assert(*old);
    lua_pop(L, 1);
    lua_pushcfunction(L, _new);
    lua_rawset(L, -3);
}

int
thread::init(lua_State *L)
{
    int rc;
    luaL_Reg lib[] = {
        { "spawn", lspawn },
        { "wait", lwait },
        { NULL, NULL }
    };

    lua_createtable(L, 0, ARRAY_SIZE(lib) - 1);
    luaL_register(L, NULL, lib);
    lua_setfield(L, -2, "thread");

    lua_getglobal(L, "coroutine");
    assert(LUA_TTABLE == lua_type(L, -1));

    //hook(L, "create", &coro_create, lcreate);
    hook(L, "resume", &coro_resume, lresume);
    hook(L, "yield", &coro_yield, lyield);

    lua_pop(L, 1); // pop "coroutine"

    return EOK;
}

} // lua
} // jet
