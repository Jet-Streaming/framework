#ifndef SRC_CORE_JET_LUA_PLUGIN_H_
#define SRC_CORE_JET_LUA_PLUGIN_H_

#include "jlua.h"
#include "jlelement.h"

typedef struct {
    jlelement_class_t **elements;
    int(*init)();
} jlplugin_t;

#endif // SRC_CORE_JET_LUA_PLUGIN_H_