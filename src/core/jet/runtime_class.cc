#include "runtime_class.h"
#include "literal.h"

namespace jet {

static rt_class *class_ht[256];
static rt_class *pending_classes;

rt_classinit::rt_classinit(rt_class* new_class) 
{
    new_class->next = pending_classes;
    pending_classes = new_class;
}

rt_classinit::~rt_classinit()
{
    ;
}

int rt_classinit::regist()
{
    return EOK;
}

bool 
rt_class::is_kind_of(const rt_class *base) const
{
    const rt_class *d;

    for (d = this; d; d = d->base) {
        if (d == base) {
            return true;
        }
    }
    return false;
}

rt_class *
rt_class::get(const char *name)
{
    rt_class *rc;

    for (rc = pending_classes; rc; rc = rc->next) {
        if (0 == strcmp(name, rc->name)) {
            return rc;
        }
    }
    return nullptr;
}

} // jet