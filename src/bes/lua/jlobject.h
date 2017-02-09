#ifndef _LOBJECT_H
#define _LOBJECT_H

#include "jlua.h"
#include <jet/object.h>

struct jlobject_s {
    jobject_t parent;
};

struct jlobject_class_s {
    jobject_class_t parent;
};

J_DECLARE_CLASS(jlobject);

//jobject_init(&jltcp_class, client, L, &jtcp_class, NULL);
//static int
//jlobject_init(void *vobjc, void *self, lua_State *L, void *, ...)
//{
//    jobject_class_t *objc;
//    va_list args;
//    int rc;
//
//    //objc = (jobject_class_t *)&name##_class;
//    objc = vobjc;
//    va_start(args, self);
//    rc = objc->constructor(self, args);
//    va_end(args);
//    return rc;
//}

#endif // _LOBJECT_H