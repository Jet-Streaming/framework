#include "bin.h"
//#include "log.h"
//#include "event.h"
//#include "task.h"
//#include "pad.h"

namespace jet {

bin::bin()
{
}

bin::~bin()
{
}

int 
bin::add(element *e)
{
    if (e == this) {
        return -EINVAL;
    }

    //if (elm->parent_element) {
    //    return -EINVAL;
    //}
    //elm->parent_element = selfelm;

    //children.push_back(e);

    return EOK;
}

} // namespace jet