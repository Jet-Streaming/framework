/*
 * types.h
 *
 *  Created on: Jul 4, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_TYPES_H_
#define SRC_CORE_JET_TYPES_H_

#include <stdint.h>
#include <errno.h>
#include <stddef.h>

//#define J_TYPEDEFINE_CLASS(name) \
//    typedef struct name##_s name##_t; \
//    typedef struct name##_class_s name##_class_t \
//
////typedef enum jerr_e jerr_t;
//
//typedef struct jlist_head_s jlist_head_t;
//typedef struct jhlist_head_s jhlist_head_t;
//typedef struct jhlist_node_s jhlist_node_t;

/* us */
typedef uint64_t jtime_t;





//typedef struct jlog_category_s jlog_category_t;
//typedef enum jlog_level_e jlog_level_t;

//typedef struct jplugin_s jplugin_t;
//
//typedef enum jpixel_format_e jpixel_format_t;


//typedef struct jobject_s jobject_t;
//typedef struct jobject_class_s jobject_class_t;
//J_TYPEDEFINE_CLASS(jobject);
//typedef struct jproperty_s jproperty_t;

//typedef struct cJSON cJSON;
//typedef struct jregistry_s jregistry_t;


#ifdef _WIN32
//#define container_of(ptr, type, member) \
//  ((type *) ((char *) (ptr) - offsetof(type, member)))
//#else
//#define container_of(ptr, type, member) ({ \
//     const typeof( ((type *)0)->member ) *__mptr = (ptr); \
//     (type *)( (char *)__mptr - offsetof(type,member) );})

#ifndef container_of
#define container_of CONTAINING_RECORD
#endif

#endif

/* Force a compilation error if condition is true, but also produce a
   result (of value 0 and type size_t), so the expression can be used
   e.g. in a structure initializer (or where-ever else comma expressions
   aren't permitted). */
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a) \
    BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(typeof(a), typeof(&a[0])))

#ifdef _WIN32
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#else
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
#endif

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#define EOK 0
//enum jerr_e {
    //J_PENDING,

    //J_ERROR     = -EPERM,
    //J_EAGAIN     = -EAGAIN,
    //J_EEXIST     = -EEXIST,
    //J_EINVAL     = -EINVAL,
    ////J_EBADFD     = -EBADFD,
    //J_ENOMEM     = -ENOMEM,
//};

#define JMIN(x, y) ({               \
    typeof(x) _min1 = (x);          \
    typeof(y) _min2 = (y);          \
    (void) (&_min1 == &_min2);      \
    _min1 < _min2 ? _min1 : _min2; })

#define JMAX(x, y) ({               \
    typeof(x) _max1 = (x);          \
    typeof(y) _max2 = (y);          \
    (void) (&_max1 == &_max2);      \
    _max1 > _max2 ? _max1 : _max2; })

#endif /* SRC_CORE_JET_TYPES_H_ */
