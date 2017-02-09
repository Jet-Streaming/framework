#pragma once
#include "../jcache.h"
#include <lua/jlelement.h>
//#include <memory>
#include <string>

namespace jet {
namespace jcache {

class ljcache : public jcache, public lua::lelement
{
public:
    ljcache();
    ~ljcache();

private:

private:
};


} // jcache
} // jet