
#include "object.h"
//#include "lua/lua.h"

//static jobject_class_t jobject_class_;
//
//static int
//jobject_constructor(void *s, va_list args)
//{
//    jobject_t *self;
//
//    self = s;
//    self->_class = &jobject_class;
//    return EOK;
//}
//
//void
//jobject_class_init()
//{
//    jobject_class_t *self;
//
//    self = &jobject_class_;
//    assert(!self->inited);
//    self->constructor = jobject_constructor;
////    _class.to_string = to_string;
//    self->tname = "jobject";
//    self->inst_size = sizeof(jobject_t);
//    self->inited = 1;
//}

namespace jet {

//object_data::~object_data() {}

//object::object()
//	: d_ptr(new object_private)
//{
//	d_ptr->j_ptr = this;
//}

object::object(object_private *dd)
	: d_ptr(dd)
{
    J_D(object);
	d_ptr->j_ptr = this;
}

object::~object()
{
	J_D(object);

    // TODO
	//delete d_ptr;
}

} // namespace jet

