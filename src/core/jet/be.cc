#include "be.h"

namespace jet {

//static be *be_pending;

be::be(const char *name, const char *desc)
{
    register_be(this);
}

be::~be()
{
}

//void
//be::regist()
//{
//    assert(!current_be);
//    current_be = this;
//}

} // namespace jet