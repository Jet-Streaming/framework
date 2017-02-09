#pragma once
//#include "jet.h"
//#include "hash.h"
//#include "array.h"
#include "pad.h"
//#include <map>
//#include "sys/queue.h"

namespace jet {

//template class J_EXTERN std::allocator<jet::pad *>;
//template class J_EXTERN std::_Vector_alloc<std::_Vec_base_types<pad *, std::allocator<pad *> >;
//template class J_EXTERN std::vector<pad *>;
//template class J_EXTERN std::list<pad *>;

class element_private;
class J_EXTERN element : public object
{
	J_DECLARE_PRIVATE(element);

public:
    //element();
	element(element_private *dd = nullptr);
	virtual ~element();
	element(const element &) = delete;
	element &operator =(const element &) = delete;

    void add_pad(pad *p);
    int link(const char *mypad, element *elm, const char *pad);
    void abort();
	
	//virtual const char *klass() = 0;

    //typedef element* (*element_creator)();

    //static element* create(const char *klass);
    //static int reg(const char *klass, element_creator creator);

private:
    pad *find_pad(const char *name);
};

} // namespace jet

//struct jelement_class_s {
//    jobject_class_t parent;
//
//    const char *name;
//    jplugin_t *plugin;
//    jhlist_node_t elements;
//
//    jcapability_t **srccaps;
//    int nsrccaps;
//    jcapability_t **sinkcaps;
//    int nsinkcaps;
//
//    int(*to_registry)(jelement_t *self, jregistry_t *reg);
//    int(*change_state)(jelement_t *self, jelement_transition_t trans);
//
//    int(*pre_loop)(jelement_t *self);
//    int(*post_loop)(jelement_t *self);
//    //    int (*loop)(jelement_t *self); // wait together with task fd
//    int(*init_seginfo)(jelement_t *self, jpad_t *pad);
//    int(*update_seginfo)(jelement_t *self, jpad_t *pad);
//};


//int
//jelement_register(jplugin_t *plugin, jelement_class_t *_class);

//int
//jelement_set_state(jelement_t *self, jelement_state_t state);
//int
//jelement_change_state(jelement_t *self, jelement_transition_t trans);

//int
//jelement_add_pad(jelement_t *self, jpad_t *pad);
//int
//jelement_new_task(jelement_t *self);

//int
//jelement_link(jelement_t *elm1, const char *pad1, jelement_t *elm2, const char *pad2);

//void
//jelement_terminate(jelement_t *self);

//// 根据其他一个或几个pad的seginfo初始化$pad的seginfo，所依据的pad由element根据自身特点决定
//static int
//jelement_init_seginfo(jelement_t *self, jpad_t *pad)
//{
//    jelement_class_t *kls;
//
//    kls = J_CLASS(self);
//    return kls->init_seginfo(self, pad);
//}
//
//// 当$pad的seginfo改变时，通知element。element一般是继续通知其他相关的pad
//static int
//jelement_update_seginfo(jelement_t *self, jpad_t *pad)
//{
//    jelement_class_t *kls;
//
//    kls = J_CLASS(self);
//    return kls->update_seginfo(self, pad);
//}


//static inline int
//jelement_prepare(jelement_t *self)
//{
//    return jelement_set_state(self, JELEMENT_STATE_READY);
//}
//
//static inline int
//jelement_pause(jelement_t *self)
//{
//    return jelement_set_state(self, JELEMENT_STATE_PAUSED);
//}
//
//static inline int
//jelement_play(jelement_t *self)
//{
//    return jelement_set_state(self, JELEMENT_STATE_PLAYING);
//}
//
//static inline int
//jelement_stop(jelement_t *self)
//{
//    return jelement_set_state(self, JELEMENT_STATE_READY);
//}
