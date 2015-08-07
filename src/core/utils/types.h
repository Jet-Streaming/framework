/*
 * types.h
 *
 *  Created on: Jul 4, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_TYPES_H_
#define SRC_CORE_JET_TYPES_H_


#define J_TYPEDEFINE_CLASS(name) \
    typedef struct j##name##_s j##name##_t; \
    typedef struct j##name##_class_s j##name##_class_t \

typedef enum jerr_e jerr_t;

typedef struct jlist_head_s jlist_head_t;
typedef struct jhlist_head_s jhlist_head_t;
typedef struct jhlist_node_s jhlist_node_t;

typedef struct jlog_category_s jlog_category_t;
typedef enum jlog_level_e jlog_level_t;

typedef struct jplugin_s jplugin_t;

typedef enum jpixel_format_e jpixel_format_t;

typedef struct jproperty_s jproperty_t;

typedef struct cJSON cJSON;
typedef struct jregistry_s jregistry_t;

typedef enum jelement_state_e jelement_state_t;
typedef enum jelement_state_transition_e jelement_state_transition_t;

J_TYPEDEFINE_CLASS(bin);
J_TYPEDEFINE_CLASS(capability);
J_TYPEDEFINE_CLASS(basesrc);
J_TYPEDEFINE_CLASS(basesink);
J_TYPEDEFINE_CLASS(basetransform);
J_TYPEDEFINE_CLASS(object);
J_TYPEDEFINE_CLASS(pad);
J_TYPEDEFINE_CLASS(pipeline);
J_TYPEDEFINE_CLASS(videocap_xraw);
J_TYPEDEFINE_CLASS(videocap);
J_TYPEDEFINE_CLASS(element);
J_TYPEDEFINE_CLASS(buffer);
//J_TYPEDEFINE_CLASS();
//J_TYPEDEFINE_CLASS();
//J_TYPEDEFINE_CLASS();
//J_TYPEDEFINE_CLASS();
//J_TYPEDEFINE_CLASS();
//J_TYPEDEFINE_CLASS();
//J_TYPEDEFINE_CLASS();
//J_TYPEDEFINE_CLASS();

#define container_of(ptr, type, member) ({ \
     const typeof( ((type *)0)->member ) *__mptr = (ptr); \
     (type *)( (char *)__mptr - offsetof(type,member) );})

/* Force a compilation error if condition is true, but also produce a
   result (of value 0 and type size_t), so the expression can be used
   e.g. in a structure initializer (or where-ever else comma expressions
   aren't permitted). */
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a) \
    BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(typeof(a), typeof(&a[0])))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

#endif /* SRC_CORE_JET_TYPES_H_ */
