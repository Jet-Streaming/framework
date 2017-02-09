//#include "../jet.h"
#include "jlplugin.h"
#include <jet/plugin.h>
#include <jet/element.h>
#include <jet/registry.h>
#include "jlelement.h"

static jhlist_head_t elements[1024];

struct jlplugin_element_s {
    jhlist_node_t node;
    jlelement_class_t *element;
};

int
jlplugin_load(jplugin_t *plugin)
{
    int i;
    jplugin_be_t **be;
    jlplugin_t *lplugin;
    jlelement_class_t *elm;
    uint32_t hash;

    be = plugin->bes;
    while (*be) {
        if (!strcmp("Lua", (*be)->name)) {
            break;
        }
        ++be;
    }

    if (!be) {
        return -EINVAL;
    }

    lplugin = (*be)->ctx;
    i = lplugin->init();
    if (i) {
        return i;
    }

    // insert elements into hash table
    for (i = 0; lplugin->elements[i]; ++i) {
        elm = lplugin->elements[i];
        hash = hlist_hash_key(elm->name, strlen(elm->name));
        hash %= ARRAY_SIZE(elements);
        hlist_add_head(&elm->elements, &elements[hash]);
    }

    return EOK;
}

static jlelement_class_t *
find_element_class(const char *name)
{
    jlelement_class_t *elm;
    uint32_t hash;
    jhlist_node_t *ln;

    hash = hlist_hash_key(name, strlen(name));
    hash %= ARRAY_SIZE(elements);
    ln = elements[hash].first;
    elm = NULL;
    while (ln) {
        elm = container_of(ln, jlelement_class_t, elements);
        if (0 == strcmp(name, elm->name)) {
            break;
        }
        ln = ln->next;
    };

    return elm;
}

static jlelement_class_t *
get_element_class(const char *name)
{
    jlelement_class_t *elm;
    int rc;
    const char *pname;
    jplugin_t *plugin;

    elm = find_element_class(name);
    if (!elm) {
        pname = jreg_get_plugin(name);
        if (!pname) {
            return NULL;
        }
        rc = jplugin_load(pname, &plugin);
        if (rc) {
            return NULL;
        }
        rc = jlplugin_load(plugin);
        if (rc) {
            return NULL;
        }
        elm = find_element_class(name);
    }

    return elm;
}

jelement_t *
_create_element(lua_State *L, const char *url)
{
    jelement_class_t *jcls;
    jlelement_class_t *lcls;
    jobject_class_t *objc;
    //jelement_t *jelm;
    jlelement_t *lelm;
    int rc;
    lua_State *mainL;
    //va_list args[3];

    jcls = jplugin_get_element_class(url);
    if (!jcls) {
        return NULL;
    }

    //jelm = jplugin_create_element(url);
    //if (!jelm) {
    //    return NULL;
    //}

    lcls = get_element_class(url);
    if (!lcls) {
        return NULL;
    }
    mainL = jlua_ctx()->L;
    lelm = lua_newuserdata(mainL, lcls->inst_size);
    if (!lelm) {
        return NULL;
    }
    objc = (jobject_class_t *)lcls;

    //args[0] = L;
    //args[1] = jcls;
    //args[2] = NULL;
    lelm->jet_inst_class = jcls;
    rc = objc->constructor(lelm, NULL);
    lua_xmove(mainL, L, 1);
    if (EOK != rc) {
        // destruct it
        return NULL;
    }

    ////pobjc = objc->parent_class;
    //luaL_getmetatable(L, objc->tname);
    //lua_setmetatable(L, -2);

    //// TODO set to _G, don't create
    //lua_createtable(L, 0, 0);
    //rc = lua_setfenv(L, -2);
    //assert(1 == rc);

    return lelm;
}

static int
create_element(lua_State *L)
{
    int n;
    jlelement_t *elm;
    jobject_class_t *kls;
    const char *url;
    jlua_ctx_t *ctx;

    //n = lua_gettop(L);
    //if (n != 1) {
    //    return luaL_error(L, "invalid arguments");
    //}

    url = luaL_checkstring(L, 1);
    assert(url);

    elm = _create_element(L, url);
    if (!elm) {
        return luaL_error(L, "failed");
    }

    // userdata don't have keys
    //lua_pushinteger(L, 11);
    //lua_setfield(L, -2, "read_start");

    //kls = J_CLASS(elm);
    //kls = kls->parent_class;
    //assert(kls);
    //luaL_getmetatable(L, kls->tname);
    //lua_setmetatable(L, -2);

    //n = lua_gettop(L);

    //// save into elements table
    //ctx = jlua_ctx();
    ////n = lua_gettop(L);
    //lua_rawgeti(L, LUA_REGISTRYINDEX, ctx->elements_table);
    ////n = lua_gettop(L);
    //assert(LUA_TTABLE == lua_type(L, -1));
    //lua_pushlightuserdata(L, elm);
    ////n = lua_gettop(L);
    //lua_pushvalue(L, -3);
    ////n = lua_gettop(L);
    //lua_rawset(L, -3);
    ////n = lua_gettop(L);
    //lua_pop(L, 1); // pop ctx->elements_table

    //lua_getfenv(L, -1);
    //lua_traverse_table(L, -2);
    //lua_pop(L, 1);

    // create env table
    //lua_createtable(L, 0, 0);
    //n = lua_setfenv(L, -2);
    //assert(1 == n);

    //lua_getfenv(L, -1);
    //lua_traverse_table(L, -2);
    //lua_pop(L, 1);

    //n = lua_gettop(L);
    return 1;
}

int
jlua_plugin_init(lua_State *L)
{
    luaL_Reg lib[] = {
        { "create_element", create_element },
        { NULL, NULL }
    };

    lua_createtable(L, 0, 3 /* nrec */);    /* jet.plugin */
    luaL_register(L, NULL, lib);
    lua_setfield(L, -2, "plugin");

    return EOK;
}