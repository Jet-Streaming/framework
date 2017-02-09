#include "tcp.h"
//#include <jet/packet.h>
#include <jet/literal.h>

namespace jet {
namespace coremodule {

tcp::tcp()
{
}

tcp::~tcp()
{
}

//static std::map<literal, tcp::impl_allocator> impls;
//
//void 
//tcp::impl_register(const char *name, impl_allocator a)
//{
//	assert(0 == impls.count(name));
//	impls[name] = a;
//}
//
//tcp *
//tcp::impl_new(const char *name)
//{
//	auto iter = impls.find(name);
//	if (impls.end() == iter) {
//		return nullptr;
//	}
//	else {
//		return (*iter->second)();
//	}
//}


} // coremodule
} // jet

//static int
//inst_init(jtcp_t *self, va_list args)
//{
//    strncpy(pad->name, "data", sizeof(pad->name));
//    //pad->seginfo.len = 0;
//    //pad->seg_tailroom = 0;
//    rc = jelement_add_pad(elm, pad);
//}
