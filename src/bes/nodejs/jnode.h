#pragma once
//# undef BUILDING_V8_SHARED
//# define USING_V8_SHARED 1

#include <jet/be.h>

#if defined(BUILDING_JET_EXTENSION) && !defined(NODE_WANT_INTERNALS)
# undef BUILDING_V8_SHARED
# define USING_V8_SHARED 1
#endif
#include <v8.h>

//#define JV8_EXPORT extern "C" J_EXPORT

namespace jet {
namespace jnodejs {

class V8_EXPORT jnodejs : public be
{
public:
	jnodejs();
    ~jnodejs();

    // util
    //static int report(lua_State *L, int status);
    //static void traverse_table(lua_State *L, int index);

private:

    int start(int argc, char *argv[]);

    //lua_State *lstate;
};

//class LUA_API initializer
//{
//public:
//
//    typedef int(*handler)(lua_State *L);
//    enum priority {
//        FRAMEWORK = 0x80,
//        ELEMENT = 0x100,
//    };
//
//    initializer(handler h, priority p = ELEMENT);
//    ~initializer();
//
//    static int initialize(lua_State *L);
//
//private:
//    handler h;
//    priority p;
//    initializer *next;
//
//    void regist();
//};


} // jnodejs
} // jet