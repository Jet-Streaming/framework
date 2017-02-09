#include "module.h"
#include <jet/module.h>
//#include "identity.h"
//#include "fakesinkpad.h"
//#include "fakesink.h"
//#include "tcpsrc.h"
//#include "queuepad.h"
//#include "queue.h"
//#include "tcp.h"
//#include "ostcp.h"
//
//#include "jlplugin.h"
//#include "lua/ltcp.h"
//


//
//// Lua
//static jlelement_class_t *lelements[8];
//
//static int
//lua_init()
//{
//    jltcp_class_init();
//
//    lelements[0] = jltcp_class();
//
//    return EOK;
//}
//
//static jlplugin_t lplugin = {
//    .elements = lelements,
//    .init = lua_init,
//};
//
//static jplugin_be_t be_lua = {
//    .name = "Lua",
//    .ctx = &lplugin,
//};
//
//// plugin
//static jelement_class_t *elements[8];
//
//static int
//plugin_init()
//{
//    int i;
//    void *p;
//
//    //p = jelement_class();
//
//    //jidentity_class_init();
//    //jfakesinkpad_class_init();
//    //jfakesink_class_init();
//    //jtcpsrc_class_init();
//    //jqueuepad_class_init();
//    //jqueue_class_init();
//    jtcp_class_init();
//    jostcp_class_init();
//
//    //i = 0;
//    //elements[i++] = (jelement_class_t *)&jidentity_class;
//    //elements[i++] = (jelement_class_t *)&jfakesink_class;
//    //elements[i++] = (jelement_class_t *)&jtcpsrc_class;
//    //elements[i++] = (jelement_class_t *)&jqueue_class;
//    //elements[i++] = (jelement_class_t *)&jtcp_class;
//    elements[0] = jostcp_class();
//
//    //void *p = &jtcp_class;
//    //i = 0;
//    //lelements[i++] = ;
//
//    return EOK;
//}
//
//static jplugin_be_t *bes[] = {
//    &be_lua,
//    NULL
//};
//
//__declspec(dllexport) jplugin_t jthis_plugin = {
//        .name = "coreplugin",
//        .licence = "MPL",
//        .elements = elements,
//        .init = plugin_init,
//
//        .bes = bes,
//};

namespace jet {
namespace coremodule {


//static module _module("coremodule");
//
//JET_C_CTOR(_register_coremodule) {
//    module::regist(&_module);
//}

#if 0
void 
TCPWrap::Initialize(Local<Object> target,
    Local<Value> unused,
    Local<Context> context)
{
}

NODE_MODULE_CONTEXT_AWARE_BUILTIN(tcp_wrap, node::TCPWrap::Initialize)
#endif

} // coremodule
} // jet