#pragma once
#include "../ostcp.h"
#include <lua/jlelement.h>

namespace jet {
namespace coremodule {

class lostcp : public ostcp, public lua::lelement
{
public:
	lostcp();
	~lostcp();

private:

};


} // coremodule
} // jet