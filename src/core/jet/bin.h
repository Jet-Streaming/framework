#pragma once
//#include "jet.h"
#include "element.h"
//#include "array.h"

namespace jet {

class J_EXTERN bin : public element
{
public:
    bin();
    ~bin();

    int add(element *e);

private:
    //std::vector<element *> children;
};


} // namespace jet