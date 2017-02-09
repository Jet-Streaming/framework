#include <nodejs/jnode.h>

using v8::Array;
using v8::Context;
using v8::EscapableHandleScope;
using v8::External;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::HandleScope;
using v8::Integer;
using v8::Local;
using v8::Object;
using v8::PropertyAttribute;
using v8::PropertyCallbackInfo;
using v8::String;
using v8::Undefined;
using v8::Value;

void foo()
{
	Local<FunctionTemplate> t;
	t->InstanceTemplate()->SetInternalFieldCount(1);
}
