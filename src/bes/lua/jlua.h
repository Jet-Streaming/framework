#pragma once

//#define lua_c
//#define loslib_c
//#define LUA_LIB
//#undef LUA_BUILD_AS_DLL

//#undef BUILDING_UV_SHARED
//#define USING_UV_SHARED 1

#define JLUA_EXPORT extern "C" J_EXPORT

#include <jet/be.h>

extern "C" {

//#define LUAI_EXTRASPACE 0
//static void luai_userstateresume(void *_L, int nargs){}
//static void luai_userstateyield(void *_L, int nargs){}

#include <lauxlib.h>
#include <lualib.h>
}

namespace jet {
namespace lua {

class LUA_API jlua : public be
{
public:
    jlua();
    ~jlua();

    //int load_module(module *m);
    //static lua_State *L();

    // util
    static int report(lua_State *L, int status);
    static void traverse_table(lua_State *L, int index);

private:

    int start(int argc, char *argv[]);

    lua_State *lstate;
};

class LUA_API initializer
{
public:

    typedef int(*handler)(lua_State *L);
    enum priority {
        FRAMEWORK = 0x80,
        ELEMENT = 0x100,
    };

    initializer(handler h, priority p = ELEMENT);
    ~initializer();

    static int initialize(lua_State *L);

private:
    handler h;
    priority p;
    initializer *next;

    void regist();
};


} // lua
} // jet

//typedef struct {
//    lua_State *L;
//    //int elements_table;
//    //const char *init_file;
//    int waiting_corotines;
//    jlelement_t *current_element;
//} jlua_ctx_t;

