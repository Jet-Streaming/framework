#pragma once
#include "jlua.h"
#include <jet/element.h>

namespace jet {
namespace lua {

class thread;
class LUA_API lelement
{
public:
    lelement();
    ~lelement();

    //class event {
    //    const char *evt;
    //    //jarray_t refs;
    //    int ref;
    //};


//TODO protected:
    // TODO uthread set
    thread *waiting_thread;

private:
    //std::vector<int> events;
};

} // lua
} // jet

//
//struct jlelement_s {
//    jlobject_t parent;
//    union {
//        jelement_t *jet_inst;
//        jelement_class_t *jet_inst_class;
//    };
//    uint16_t async_state; // async operation state
//
//    jarray_t events;
//    int on_error;
//};
//
//struct jlelement_class_s {
//    jlobject_class_t parent;
//
//    const char *name;
//    size_t inst_size;
//    jhlist_node_t elements;
//};
//
//J_DECLARE_CLASS(jlelement);
//
//
//int
//jlelement_push_listener(jlelement_t *self, const char *event);
