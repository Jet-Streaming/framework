#include "jnode.h"
//#include "jlthread.h"
//#include <sys/queue.h>
//#include <v8-platform.h>
//
//#include <libplatform/libplatform.h>
//#if NODE_USE_V8_PLATFORM
//#include "libplatform/libplatform.h"
//#endif  // NODE_USE_V8_PLATFORM
#include "node.h"

namespace jet {
namespace jnodejs {

//using v8::Array;
//using v8::ArrayBuffer;
//using v8::Boolean;
//using v8::Context;
//using v8::EscapableHandleScope;
//using v8::Exception;
//using v8::Float64Array;
//using v8::Function;
//using v8::FunctionCallbackInfo;
//using v8::HandleScope;
//using v8::HeapStatistics;
//using v8::Integer;
//using v8::Isolate;
//using v8::Local;
//using v8::Locker;
//using v8::MaybeLocal;
//using v8::Message;
//using v8::Name;
//using v8::Null;
//using v8::Number;
//using v8::Object;
//using v8::ObjectTemplate;
//using v8::Promise;
//using v8::PromiseRejectMessage;
//using v8::PropertyCallbackInfo;
//using v8::ScriptOrigin;
//using v8::SealHandleScope;
//using v8::String;
//using v8::TryCatch;
//using v8::Uint32Array;
//using v8::V8;
//using v8::Value;

static jnodejs
this_be;

jnodejs::jnodejs() :
	be("v8", nullptr)
{
}

jnodejs::~jnodejs()
{
}

int
jnodejs::start(int argc, char *argv[])
{
    int rc;

	rc = node::Start(argc, argv);
    return rc;
}

//initializer::initializer(handler _h, priority _p) :
//    h(_h), p(_p), next(nullptr)
//{
//    regist();
//}
//
//initializer::~initializer()
//{
//}
//
//static initializer *initializers = nullptr;
//void
//initializer::regist()
//{
//    initializer *i, *prev;
//
//    for (i = initializers, prev = nullptr; i; i = i->next) {
//        prev = i;
//        if (this->p >= i->p) {
//            break;
//        }
//    }
//    if (prev) {
//        this->next = prev->next;
//        prev->next = this;
//    } else {
//        initializers = this;
//    }
//}
//
//int 
//initializer::initialize(lua_State *L)
//{
//    int rc = EOK;
//    for (initializer *i = initializers; i; i = i->next) {
//        rc = i->h(L);
//        if (rc) {
//            break;
//        }
//    }
//    initializers = nullptr;
//    return rc;
//}


// utils

} // jnodejs
} // jet