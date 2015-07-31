/*
 * plugin.c
 *
 *  Created on: Jul 4, 2015
 *      Author: root
 */


#include "plugin.h"
#include "limits.h"
#include "hash.h"
#include "element.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static jhlist_head_t elements[1024];

struct jplugin_element_s {
    jhlist_node_t node;
    jelement_class_t *element;
};

jerr_t
jplugin_init()
{
    return J_OK;
}

jerr_t
jplugin_load(const char *file)
{
    void *dll;
    char plugin_name[J_MAX_PLUGIN_NAME];
    jplugin_t *plugin;
    char *s, *e;
    jerr_t rc;
    int i;
    jelement_class_t *elm;
    uint32_t hash;

    dll = dlopen(file, RTLD_LAZY);
    if (!dll) {
        goto err;
    }

    s = strrchr(file, '/');
    s += 4; // /lib
    e = strchr(s, '.');
    memset(plugin_name, 0, sizeof(plugin_name));
    plugin_name[0] = 'j';
    memcpy(plugin_name+1, s, e-s);
    strcat(plugin_name, "_plugin");

    plugin = dlsym(dll, plugin_name);
    if (!plugin) {
        goto err;
    }

    rc = plugin->init();
    if (J_OK != rc) {
        goto err;
    }

    // insert elements into hash table
    for (i = 0; plugin->elements[i]; ++i) {
        elm = plugin->elements[i];
        hash = hlist_hash_key(elm->name, strlen(elm->name));
        hash %= ARRAY_SIZE(elements);
        hlist_add_head(&elm->elements, &elements[hash]);
    }

    return J_OK;

err:
    if (dll) {
        (void)dlclose(dll);
    }
    return J_ERROR;
}

jerr_t
jplugin_unload(const char *plugin)
{
    return J_OK;
}

jerr_t
jplugin_scan_load()
{
    return J_OK;
}

jelement_class_t *
jplugin_get_element_class(const char *name)
{
    jelement_class_t *elm;
    uint32_t hash;
    jhlist_node_t *ln;

    hash = hlist_hash_key(name, strlen(name));
    hash %= ARRAY_SIZE(elements);
    ln = elements[hash].first;
    while (ln) {
        elm = container_of(ln, jelement_class_t, elements);
        if (0 == strcmp(name, elm->name)) {
            break;
        }
        ln = ln->next;
    };

    return elm;
}

jelement_t *
jplugin_create_element(const char *name)
{
    jelement_class_t *_class;
    jobject_class_t *objc;
    jelement_t *elm;

    _class = jplugin_get_element_class(name);
    if (!_class) {
        return NULL;
    }

    elm = malloc(_class->inst_size);
    objc = (jobject_class_t *)_class;
    objc->constructor(elm);

    return elm;
}










