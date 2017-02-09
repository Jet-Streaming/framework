#pragma once
#include "jlua.h"
//#include "jlelement.h"

namespace jet {
namespace lua {

class lelement;
class LUA_API thread
{
public:
    thread() 
        : ref(LUA_NOREF)
        //, L(_L)
        , cleanup(nullptr)
        , cleanup_userdata(nullptr)
        , thrd(0)
        , csid(0)
    {}
    ~thread() {}

    static int init(lua_State *L);

    lua_State *from_thread();
    static thread *to_thread(lua_State *L);

    void set_element(lelement *e);
    lelement *get_element();

    //static uint32_t csid(lua_State *L);
    int resume(int narg);
    int yield(int nresults);
    static int pcall(lua_State *L, int nargs, int nresults);

    typedef void(*cleanup_cb)(void *p);
    static void set_cleanup(lua_State *L, cleanup_cb cb, void *userdata);

//private:
    int ref;
    //lua_State *L;
    cleanup_cb cleanup;
    void *cleanup_userdata;

    uint32_t csid : 24; // context switch ID
    uint32_t thrd : 1;
};

// 一系列thread（或context）的集合，一般对应一个element，也可对应global，
// 当该对象销毁时，它包含的所有thread都应该被kill
//class LUA_API run_context : public thread
//{
//
//private:
//
//};


//int
//jlua_thread_init(lua_State *L);
//
//
//uint32_t
//jlthread_csid(lua_State *L);
//int
//jlthread_resume(lua_State *L, int narg);
//int
//jlthread_yield(lua_State *L, void *ctx, int nresults);
//int
//jlthread_pcall(lua_State *L, int nargs, int nresults);
//
//
////J_EXTERN int
////luaJ_thread_set(lua_State *co, void *k);
////J_EXTERN lua_State *
////luaJ_thread_get(void *k);
//
//void
//jlthread_

} // lua
} // jet
