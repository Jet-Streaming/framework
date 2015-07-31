
#ifndef JET_OBJECT_H_
#define JET_OBJECT_H_

#include <jet/jet.h>


struct jproperty_s {
    uint16_t offset;
    void *_class;
    const char *name;
    const char *description;

    uint32_t readable : 1;
    uint32_t writable : 1;
};

struct jobject_s {
    void *_class;
};

struct jobject_class_s {
    void *parent_class;

    uint32_t inited : 1;
    uint32_t clear : 1;

    jproperty_t *properties;
    uint8_t nproperties;

    // virtual functions
    void (*constructor)(void *self);
    void (*destructor)(void *self);
    int (*to_string)(void *self, char *buf, size_t len);
};

//#define JOBJECT_CLASS (&jobject_class)

extern jobject_class_t jobject_class;

//static void
//jobject_construct(jobject_t *self)
//{
//    jobject_class.constructor(self);
//}
//
//static void
//jobject_destruct(jobject_t *self)
//{
//    jobject_class.destructor(self);
//}

void jobject_class_init();
int jobject_is_class(void *obj, void *cls);


#endif /* JET_OBJECT_H_ */
