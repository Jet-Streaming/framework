/*
 * jet.h
 *
 *  Created on: Jun 28, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_H_
#define SRC_CORE_JET_H_

#include <stdint.h>
#include <unistd.h>
#include <assert.h>

#include <jet/types.h>

#define J_DEFINE_CLASS(name, pname, inst_init_fn, class_init_fn) \
    j##name##_class_t j##name##_class; \
    \
    static void \
    j##name##_constructor(void *s) { \
        jobject_class_t *objc, *pc; \
        jobject_t *obj; \
        j##name##_t *self; \
        \
        objc = (jobject_class_t *)&j##name##_class; \
        pc = objc->parent_class; \
        pc->constructor(s); \
        \
        obj = (jobject_t *)s; \
        obj->_class = &j##name##_class; \
        \
        /* initialize members */ \
        self = s; \
        inst_init_fn(self); \
    } \
    \
    static void \
    j##name##_destructor(void *s) { \
        ; \
    } \
    \
    void j##name##_class_init() { \
        jobject_class_t *objc, *pc; \
        \
        objc = (jobject_class_t *)&j##name##_class; \
        pc = (jobject_class_t *)&j##pname##_class; \
        assert(pc->inited); \
        assert(!objc->inited); \
        objc->parent_class = pc; \
        objc->constructor = j##name##_constructor; \
        objc->destructor = j##name##_destructor; \
        /* objc->to_string = to_string; */ \
        objc->inited = 1; \
        \
        /* initialize virtual functions */ \
        class_init_fn(&j##name##_class); \
    } \


#define J_DECLARE_CLASS(name) \
    extern j##name##_class_t j##name##_class; \
    \
    static void \
    j##name##_init(j##name##_t *self) \
    { \
        jobject_class_t *objc; \
        \
        objc = (jobject_class_t *)&j##name##_class; \
        objc->constructor(self); \
    } \
    \
    void j##name##_class_init(); \


// https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
#define J_LIKELY(expr)   (__builtin_expect (expr, 1))
#define J_UNLIKELY(expr) (__builtin_expect (expr, 0))


enum jerr_e {
    J_OK,
    J_ERROR,
    J_AGAIN,
    J_EXIST,
};


#endif /* SRC_CORE_JET_H_ */
