#include "node.h"
//#include "node_natives.h"
#include "v8.h"
#include "env.h"
#include "env-inl.h"
#include <memory>

namespace node {

using v8::HandleScope;
using v8::Local;
using v8::NewStringType;
using v8::Object;
using v8::String;

#define READ_FILE 1
//#define internal_bootstrap_node_native bes_nodejs_lib_internal_bootstrap_node_native

Local<String> MainSource(Environment* env) {
#if READ_FILE
	FILE *fp = fopen("nodejslib/internal/bootstrap_node.js", "rb");
	fseek(fp, 0, SEEK_END);
	const int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	std::unique_ptr<char[]> fs(new char[size]);
	fread((void *)fs.get(), 1, size, fp);
	fclose(fp);
#endif

  return String::NewFromUtf8(
      env->isolate(),
#if READ_FILE
	  fs.get(),
#else
	  reinterpret_cast<const char*>(internal_bootstrap_node_native),
#endif
      NewStringType::kNormal,
#if READ_FILE
	  size
#else
	  sizeof(internal_bootstrap_node_native)
#endif
	  ).ToLocalChecked();
}

#if !READ_FILE
static const char *
jetName(const char *in)
{
	static const char prefix[] = "bes/nodejs/lib/";
	if (strstr(in, prefix)) {
		in += sizeof(prefix) - 1;
	}
	return in;
}
#endif

void DefineJavaScript(Environment* env, Local<Object> target) {
  HandleScope scope(env->isolate());

#if READ_FILE
  const char *natives[] = {
	  //"internal/bootstrap_node.js",
	  "config",

	  "_debug_agent",
	  "_debugger",
	  "assert",
	  "buffer",
	  "child_process",
	  "console",
	  "constants",
	  "crypto",
	  "cluster",
	  "dgram",
	  "dns",
	  "domain",
	  "events",
	  "fs",
	  "http",
	  "_http_agent",
	  "_http_client",
	  "_http_common",
	  "_http_incoming",
	  "_http_outgoing",
	  "_http_server",
	  "https",
	  "_linklist",
	  "module",
	  "net",
	  "os",
	  "path",
	  "process",
	  "punycode",
	  "querystring",
	  "readline",
	  "repl",
	  "stream",
	  "_stream_readable",
	  "_stream_writable",
	  "_stream_duplex",
	  "_stream_transform",
	  "_stream_passthrough",
	  "_stream_wrap",
	  "string_decoder",
	  "sys",
	  "timers",
	  "tls",
	  "_tls_common",
	  "_tls_legacy",
	  "_tls_wrap",
	  "tty",
	  "url",
	  "util",
	  "v8",
	  "vm",
	  "zlib",
	  "internal/child_process",
	  "internal/cluster",
	  "internal/freelist",
	  "internal/linkedlist",
	  "internal/net",
	  "internal/module",
	  "internal/process/next_tick",
	  "internal/process/promises",
	  "internal/process/stdio",
	  "internal/process/warning",
	  "internal/process",
	  "internal/readline",
	  "internal/repl",
	  "internal/socket_list",
	  "internal/util",
	  "internal/v8_prof_polyfill",
	  "internal/v8_prof_processor",
	  "internal/streams/lazy_transform",
	  "internal/streams/BufferList",
  };
  //uv_fs_t req;
  //uv_dirent_t ent;
  //uv_fs_scandir(uv_default_loop(), &req, "", 0, nullptr);
  //uv_fs_scandir_next(&req, &ent);
#endif

  for (auto native : natives) {
#if READ_FILE
	  char source_file[1024];
	  if (!strcmp(native, "config")) {
		  snprintf(source_file, sizeof(source_file), "../%s.gypi", native);
	  } else {
		  snprintf(source_file, sizeof(source_file), "nodejslib/%s.js", native);
	  }
	  FILE *fp = fopen(source_file, "rb");
	  fseek(fp, 0, SEEK_END);
	  const int source_len = ftell(fp);
	  fseek(fp, 0, SEEK_SET);
	  std::unique_ptr<char[]> source_code(new char[source_len]);
	  fread((void *)source_code.get(), 1, source_len, fp);
	  fclose(fp);
#endif

	  //if (native.source != internal_bootstrap_node_native) {
		  Local<String> name = String::NewFromUtf8(env->isolate(), 
#if READ_FILE
			  native
#else
			  jetName(native.name)
#endif
		  );
		  Local<String> source =
			  String::NewFromUtf8(
				  env->isolate(), 
#if READ_FILE
				  source_code.get(),
#else
				  reinterpret_cast<const char*>(native.source),
#endif
				  NewStringType::kNormal, 
#if READ_FILE
				  source_len
#else
				  native.source_len
#endif
			  ).ToLocalChecked();
		  target->Set(name, source);
	  //}
  }
}

}  // namespace node
