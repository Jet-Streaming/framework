#pragma once
#include "object.h"

namespace jet {

struct rt_class;
struct J_EXTERN rt_classinit {
    rt_classinit &operator =(const rt_classinit &) = delete;
    rt_classinit(const rt_classinit &) = delete;
    rt_classinit(rt_class* new_class);
    ~rt_classinit();
    static int regist();
};

struct J_EXTERN rt_class {
    const char *name;
    size_t size;
    object* (*create_object_func)(); // NULL => abstract class
    rt_class *base;
    rt_class *next;

    rt_class &operator =(const rt_class &) = delete;
    rt_class(const rt_class &) = delete;
    bool is_kind_of(const rt_class *base) const;
    static rt_class *get(const char *name);
};

#define JET_DECLARE_RTCLASS(class_name) \
public: \
    static rt_class class##class_name; \
    static const rt_class *get_class() { \
        return &class##class_name; \
    } \
    virtual rt_class *get_rtclass() const { \
        return &class##class_name; \
    } \

#define JET_IMPLEMENT_ROOT_RTCLASS(class_name) \
    rt_class class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        nullptr, \
        nullptr, \
    }; \
    static const rt_classinit classinit##class_name(&class_name::class##class_name); \

#define JET_IMPLEMENT_RTCLASS(class_name, base_name) \
    rt_class class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        nullptr, \
        &base_name::class##base_name, \
    }; \
    static const rt_classinit classinit##class_name(&class_name::class##class_name); \

} // jet