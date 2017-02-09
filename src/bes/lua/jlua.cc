#include "jlua.h"
//#include "jlplugin.h"
#include "jlthread.h"
#include <sys/queue.h>

namespace jet {
namespace lua {

static jlua 
this_be;

jlua::jlua() :
    be(LUA_RELEASE, nullptr)
{
}

jlua::~jlua()
{
}

//lua_State *
//jlua::L()
//{
//    return this_be.lstate;
//}

int 
jlua::start(int argc, char *argv[])
{
    int rc;
    lua_State *L;
    int i;

    // TODO 可以使用传入的lua_State，例如和其他部件（Qt）共用同一个Lua VM
    assert(nullptr == this_be.lstate);
    L = this_be.lstate = lua_open();
    lua_gc(L, LUA_GCSTOP, 0);  /* stop collector during initialization */
    luaL_openlibs(L);  /* open libraries */
    lua_gc(L, LUA_GCRESTART, 0);

    //luaL_register(L, "jet", lib);
    //lua_newthread(L);

    //lua_createtable(L, 0, 0); // elements table
    //lua_createtable(L, 0, 1); // metatable
    //lua_pushstring(L, "v");
    //lua_setfield(L, -2, "__mode");
    //lua_setmetatable(L, -2);
    //ctx->elements_table = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_createtable(L, 0, 0);
    lua_createtable(L, 0, 1); // metatable
    lua_pushstring(L, "v");
    lua_setfield(L, -2, "__mode");
    lua_setmetatable(L, -2);
    //ctx->waiting_corotines = luaL_ref(L, LUA_REGISTRYINDEX);


    lua_createtable(L, 0, 0);    /* jet.* */
    rc = thread::init(L);

    // args
    lua_createtable(L, 0, 0);    /* jet.args */
    for (i = 1; i != argc; ++i) {
        lua_pushinteger(L, i);
        lua_pushstring(L, argv[i]);
        lua_rawset(L, -3);
    }
    lua_setfield(L, -2, "args");

    lua_setglobal(L, "jet");

    //ctx->init_file = argv[1];

    //rc = luaL_dofile(L, fname);
    rc = luaL_loadfile(L, /*argv[1]*/"jet.lua");
    rc = lua_pcall(L, 0, LUA_MULTRET, 0);
    //rc = lua_resume(L, 0);
    //if (LUA_YIELD != rc) {
    rc = report(L, rc);

    // for detecting some BUG
    rc = lua_gc(L, LUA_GCCOLLECT, 0);

    //rc = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    assert(0);

    return rc;
}

//jbe_t jbe_lua = {
//    .name = LUA_RELEASE,
//    .version = LUA_VERSION_NUM,
//};
//

initializer::initializer(handler _h, priority _p) :
    h(_h), p(_p), next(nullptr)
{
    regist();
}

initializer::~initializer()
{
}

static initializer *initializers = nullptr;
void
initializer::regist()
{
    initializer *i, *prev;

    for (i = initializers, prev = nullptr; i; i = i->next) {
        prev = i;
        if (this->p >= i->p) {
            break;
        }
    }
    if (prev) {
        this->next = prev->next;
        prev->next = this;
    } else {
        initializers = this;
    }
}

int 
initializer::initialize(lua_State *L)
{
    int rc = EOK;
    for (initializer *i = initializers; i; i = i->next) {
        rc = i->h(L);
        if (rc) {
            break;
        }
    }
    initializers = nullptr;
    return rc;
}


// utils

static void
l_message(const char *pname, const char *msg)
{
    if (pname) fprintf(stderr, "%s: ", pname);
    fprintf(stderr, "%s\n", msg);
    fflush(stderr);
}

int
jlua::report(lua_State *L, int status)
{
    if (status && !lua_isnil(L, -1)) {
        const char *msg = lua_tostring(L, -1);
        if (msg == NULL) msg = "(error object is not a string)";
        l_message(NULL, msg);
        lua_pop(L, 1);
    }
    return status;
}

void
jlua::traverse_table(lua_State *L, int index)
{
    puts("table {");
    //lua_pushnil(L);
    //// 现在的栈：-1 => nil; index => table
    //while (lua_next(L, index))
    //{
    //    // 现在的栈：-1 => value; -2 => key; index => table
    //    // 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
    //    lua_pushvalue(L, -2);
    //    // 现在的栈：-1 => key; -2 => value; -3 => key; index => table
    //    const char* key = lua_tostring(L, -1);
    //    const char* value = lua_tostring(L, -2);

    //    printf("%s => %s\n", key, value);

    //    // 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
    //    lua_pop(L, 2);
    //    // 现在的栈：-1 => key; index => table
    //}
    // 现在的栈：index => table （最后 lua_next 返回 0 的时候它已经把上一次留下的 key 给弹出了）
    // 所以栈已经恢复到进入这个函数时的状态

    /* table is in the stack at index 't' */
    lua_pushnil(L);  /* first key */
    while (lua_next(L, index) != 0) {
        /* uses 'key' (at index -2) and 'value' (at index -1) */
        switch (lua_type(L, -2)) {
        case LUA_TSTRING:
            printf("\t$%s", lua_tolstring(L, -2, NULL));
            break;
        case LUA_TNUMBER:
            printf("\t$%f", lua_tonumber(L, -2));
            break;
        default:
            printf("\t%s", lua_typename(L, lua_type(L, -2)));
            break;
        }
        printf(" - ");
        printf("%s\n", lua_typename(L, lua_type(L, -1)));
        /* removes 'value'; keeps 'key' for next iteration */
        lua_pop(L, 1);
    }
    puts("table }");
}

} // lua
} // jet