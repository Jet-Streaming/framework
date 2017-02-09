#ifndef __EVHTP_CONFIG_H__
#define __EVHTP_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EVHTP_EXPORT
# if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__
#  define EVHTP_EXPORT __attribute__ ((visibility("default")))
# else
#  define EVHTP_EXPORT
# endif
#endif


#undef EVHTP_DISABLE_EVTHR
#undef EVHTP_DISABLE_REGEX
#undef EVHTP_DISABLE_SSL
#undef EVHTP_DISABLE_EVTHR

/* #undef EVHTP_DISABLE_EVTHR */
/* #undef EVHTP_DISABLE_REGEX */
/* #undef EVHTP_DISABLE_SSL */
/* #undef EVHTP_DISABLE_EVTHR */
/* #undef EVHTP_USE_TCMALLOC */
/* #undef EVHTP_USE_JEMALLOC */
/* #undef EVHTP_USE_TCMALLOC */

#ifdef EVHTP_USE_TCMALLOC
#include <google/tcmalloc.h>
#define malloc(size)        tc_malloc(size)
#define calloc(count, size) tc_calloc(count, size)
#define realloc(ptr, size)  tc_realloc(ptr, size)
#define free(ptr)           tc_free(ptr)
#endif

#ifdef EVHTP_USE_JEMALLOC
#define JEMALLOC_NO_DEMANGLE
#include <jemalloc/jemalloc.h>
#define malloc(size)        je_malloc(size)
#define calloc(count, size) je_calloc(count, size)
#define realloc(ptr, size)  je_realloc(ptr, size)
#define free(ptr)           je_free(ptr)
#endif


#ifdef __cplusplus
}
#endif

#endif
