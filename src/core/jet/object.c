
#include "object.h"


jobject_class_t jobject_class;


static void
jobject_constructor(void *s)
{
    jobject_t *self;

    self = s;
    self->_class = &jobject_class;
}

//static int
//to_string(jobject_t *self, char *buf, size_t len)
//{
//    return snprintf(buf, len, "%p", self);
//}



void
jobject_class_init()
{
    assert(!jobject_class.inited);
    jobject_class.constructor = jobject_constructor;
//    _class.to_string = to_string;
    jobject_class.inited = 1;
}

int
jobject_is_class(void *obj, void *cls)
{
    jobject_class_t *oc = ((jobject_t *)obj)->_class;

    while (oc) {
        if (oc == cls) {
            return 1;
        }
        oc = oc->parent_class;
    }
    return 0;
}


