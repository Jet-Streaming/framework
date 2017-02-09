#pragma once

//#ifdef _WIN32
//# ifndef BUILDING_JET_EXTENSION
//#   define J_EXTERN __declspec(dllexport)
//# else
//#   define J_EXTERN __declspec(dllimport)
//# endif
//#else
//# define J_EXTERN /* nothing */
//#endif

#ifdef BUILDING_JET_EXTENSION
# undef BUILDING_UV_SHARED
# define USING_UV_SHARED 1
#endif


#include <cstdint>
#ifdef _WIN32
#else
#include <unistd.h>
#endif

#include <stdarg.h>
#include <assert.h>

#include <sys/queue.h>
#include <uv.h>
#include "types.h"

#include <vector>
#include <list>
#include <string>
#include <map>

//#ifdef _WIN32
//#define J_LIKELY(expr)   (expr)
//#define J_UNLIKELY(expr) (expr)
//#else
//// https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
//#define J_LIKELY(expr)   (__builtin_expect (expr, 1))
//#define J_UNLIKELY(expr) (__builtin_expect (expr, 0))
//#endif

//typedef enum jelement_state_e jelement_state_t;
//typedef enum jelement_transition_e jelement_transition_t;
//
//J_TYPEDEFINE_CLASS(jbin);
//J_TYPEDEFINE_CLASS(jcapability);
//J_TYPEDEFINE_CLASS(jbasesrc);
//J_TYPEDEFINE_CLASS(jbasesink);
//J_TYPEDEFINE_CLASS(jbasetransform);
//J_TYPEDEFINE_CLASS(jpad);
//J_TYPEDEFINE_CLASS(jpipeline);
//J_TYPEDEFINE_CLASS(jvideocap_xraw);
//J_TYPEDEFINE_CLASS(jvideocap);
//J_TYPEDEFINE_CLASS(jelement);
//J_TYPEDEFINE_CLASS(jmbuf);
//J_TYPEDEFINE_CLASS(jevent);

// media types

#define J_EXTERN UV_EXTERN

#ifdef _WIN32
# define J_EXPORT __declspec(dllexport)
#elif __GNUC__ >= 4
# define J_EXPORT __attribute__((visibility("default")))
#else
# define J_EXPORT /* nothing */
#endif


namespace jet {

// 使用全局对象的构造函数
//#if defined(_MSC_VER)
//#pragma section(".CRT$XCU", read)
//#define JET_C_CTOR(fn)                                                \
//  static void __cdecl fn(void);                                       \
//  __declspec(dllexport, allocate(".CRT$XCU"))                         \
//      void (__cdecl*fn ## _)(void) = fn;                              \
//  static void __cdecl fn(void)
//#else
//#define JET_C_CTOR(fn)                                                \
//  static void fn(void) __attribute__((constructor));                  \
//  static void fn(void)
//#endif

class be;
int
register_be(be *b);

J_EXTERN int
init();
J_EXTERN int
run(int argc, char *argv[]);


} // namespace jet