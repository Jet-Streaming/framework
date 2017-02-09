
#include "module.h"
#include "limits.h"
//#include "hash.h"
#include "element.h"

#ifdef _WIN32 
#else
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#endif

//#include "registry.h"

namespace jet {

//module::module()
//{
//}
//
//module::~module()
//{
//}


static module *mod_builtin;
static module *mod_pending;
static module *mod_addon;

// TODO get it from config.h
const static uint32_t mod_version = 0x12345678;

module::module(const char *n)
	: object(new module_private)
	, name(n)
	, next(nullptr)
{}

module::~module() 
{}

int
module::regist(module *m)
{
    m->next = mod_pending;
    mod_pending = m;
    return EOK;
}

static int
do_load(module *m)
{
    //if (mod_version != m->version) {
    //    return -EINVAL;
    //}
    return EOK;
}

int
module::load()
{
    int rc;
    module *m = mod_pending;

    while (m) {
        // remove from list

        rc = do_load(m);
    }
    return EOK;
}

//int
//plugin::load(const char *filename)
//{
//    uv_lib_t lib;
//
//    const bool is_error = uv_dlopen(filename, &lib);
//
//
//    return EOK;
//}

} // namespace jet


//static jhlist_head_t elements[1024];
//
//struct jplugin_element_s {
//    jhlist_node_t node;
//    jelement_class_t *element;
//};
//
//int
//jplugin_init()
//{
//    return EOK;
//}
//
///*
//插件机制
//1. 模块定义全局变量：jplugin_$plugin，如jplugin_coreplugin
//
//动态插件：导出插件全局变量，框架负责加载和调用
//静态插件：不导出，框架从可执行文件加载和调用
//*/
//int
//jplugin_load(const char *file, jplugin_t **rplugin)
//{
//    void *dll;
//    char plugin_name[J_MAX_PLUGIN_NAME];
//    jplugin_t *plugin;
//    char *s, *e;
//    int rc;
//    int i;
//    jelement_class_t *elm;
//    uint32_t hash;
//    //jbe_t **be;
//
//#ifdef _WIN32
//    dll = GetModuleHandle(file);
//    if (!dll) {
//        dll = LoadLibrary(file);
//    }
//#else
//    dll = dlopen(file, /*RTLD_LAZY*/RTLD_NOW);
//#endif
//    rc = -EINVAL;
//    if (!dll) {
//        goto err;
//    }
//
//    // TODO: use jplugin_http instead of jthis_plugin
////    s = strrchr(file, '/');
////#ifndef _WIN32
////    s += 4; // /lib
////#endif
////    e = strchr(s, '.');
////    memset(plugin_name, 0, sizeof(plugin_name));
////    plugin_name[0] = 'j';
////    memcpy(plugin_name+1, s, e-s);
////    strcat(plugin_name, "_plugin");
//
//#ifdef _WIN32
//    plugin = (jplugin_t *)GetProcAddress(dll, "jthis_plugin");
//#else
//    plugin = dlsym(dll, plugin_name);
//#endif
//    if (!plugin) {
//        goto err;
//    }
//    if (rplugin) {
//        *rplugin = plugin;
//    }
//
//    if (plugin->inited) {
//        return EOK;
//    }
//    rc = plugin->init();
//    if (EOK != rc) {
//        goto err;
//    }
//
//    // insert elements into hash table
//    for (i = 0; plugin->elements[i]; ++i) {
//        elm = plugin->elements[i];
//        hash = hlist_hash_key(elm->name, strlen(elm->name));
//        hash %= ARRAY_SIZE(elements);
//        hlist_add_head(&elm->elements, &elements[hash]);
//    }
//
//    //be = jet_bes.elts;
//    //for (i = 0; i != jet_bes.nelts; ++i) {
//    //    rc = be[i]->load_plugin(plugin);
//    //    assert(EOK == rc);
//    //}
//    plugin->inited = 1;
//
//    return rc;
//err:
//    if (dll) {
//#ifdef _WIN32
//        FreeLibrary(dll);
//#else
//        (void)dlclose(dll);
//#endif
//    }
//    return rc;
//}
//
//int
//jplugin_unload(jplugin_t *plugin)
//{
//    return EOK;
//}
//
//int
//jplugin_scan_load()
//{
//    return EOK;
//}
//
//
//static jelement_class_t *
//find_element_class(const char *name)
//{
//    jelement_class_t *elm;
//    uint32_t hash;
//    jhlist_node_t *ln;
//
//    hash = hlist_hash_key(name, strlen(name));
//    hash %= ARRAY_SIZE(elements);
//    ln = elements[hash].first;
//    elm = NULL;
//    while (ln) {
//        elm = container_of(ln, jelement_class_t, elements);
//        if (!strcmp(name, elm->name)) {
//            break;
//        }
//        ln = ln->next;
//    };
//    return elm;
//}
//
//jelement_class_t *
//jplugin_get_element_class(const char *name)
//{
//    jelement_class_t *elm;
//    int rc;
//    const char *plugin;
//
//    elm = find_element_class(name);
//    if (!elm) {
//        // search in registry(in memory)
//        plugin = jreg_get_plugin(name);
//        if (!plugin) {
//            return NULL;
//        }
//        rc = jplugin_load(plugin, NULL);
//        if (rc) {
//            return NULL;
//        }
//        elm = find_element_class(name);
//    }
//
//
//
//    return elm;
//}
//
//// url = "192.168.144.197/transcoder"
////       "tcp" // same as "localhost/tcp"
//jelement_t *
//jplugin_create_element(const char *url)
//{
//    jelement_class_t *_class;
//    jobject_class_t *objc;
//    jelement_t *elm;
//    int rc;
//
//    _class = jplugin_get_element_class(url);
//    if (!_class) {
//        return NULL;
//    }
//
//    objc = (jobject_class_t *)_class;
//    elm = malloc(objc->inst_size);
//    if (!elm) {
//        return NULL;
//    }
//    rc = objc->constructor(elm, NULL);
//    if (EOK != rc) {
//        // destruct it
//        return NULL;
//    }
//
//    return elm;
//}
