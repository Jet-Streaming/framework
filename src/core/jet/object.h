#pragma once

#include "jet.h"

template <typename T> static inline T *jGetPtrHelper(T *ptr) { return ptr; }
template <typename Wrapper> static inline typename Wrapper::pointer jGetPtrHelper(const Wrapper &p) { return p.data(); }

#define J_DECLARE_PRIVATE(Class) \
    inline Class##_private* d_func() { return reinterpret_cast<Class##_private *>(jGetPtrHelper(d_ptr)); } \
    inline const Class##_private* d_func() const { return reinterpret_cast<const Class##_private *>(jGetPtrHelper(d_ptr)); } \
    friend class Class##_private;

//#define J_DECLARE_PRIVATE_D(Dptr, Class) \
//    inline Class##_private* d_func() { return reinterpret_cast<Class##_private *>(Dptr); } \
//    inline const Class##_private* d_func() const { return reinterpret_cast<const Class##_private *>(Dptr); } \
//    friend class Class##_private;

#define J_DECLARE_PUBLIC(Class)                                    \
    inline Class* j_func() { return static_cast<Class *>(j_ptr); } \
    inline const Class* j_func() const { return static_cast<const Class *>(j_ptr); } \
    friend class Class;

#define J_D(Class) Class##_private * const d = d_func()
#define J_J(Class) Class * const j = j_func()


namespace jet {

class object;
//struct object_data {
//    virtual ~object_data() = 0;
//};

class UV_EXTERN object_private
{
	J_DECLARE_PUBLIC(object);

public:
	object_private() {}
	virtual ~object_private(){}

	object *j_ptr;
	int dummy;
};

class UV_EXTERN object
{
    J_DECLARE_PRIVATE(object);
    //inline object_private* d_func() { return reinterpret_cast<object_private *>(jGetPtrHelper(d_ptr)); }
    //inline const object_private* d_func() const { return reinterpret_cast<const object_private *>(jGetPtrHelper(d_ptr)); }
    //friend class object_private;

public:
	object(object_private *dd);
    virtual ~object();
	object &operator =(const object &) = delete;

    //int to_string(char *buf, size_t len) = 0;
    //const char *tname;
private:

protected:
	object_private *d_ptr;
};


//struct jobject_class_s {
//    void *parent_class;
//    const char *tname;
//
//    uint32_t inited : 1;
//    uint32_t clear : 1;
//
//    size_t inst_size;
//    jproperty_t *properties;
//    uint8_t nproperties;
//
//    // virtual functions
//    int(*constructor)(void *self, va_list args);
//    void(*destructor)(void *self);
//    int(*to_string)(void *self, char *buf, size_t len);
//};


} // namespace jet