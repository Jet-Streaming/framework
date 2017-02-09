#pragma once
#include "object.h"
#include "literal.h"

namespace jet {

/* 框架层只提供module描述，不提供动作（加载等）。
 * JS：js层提供module管理，c++层提供加载
 * Lua：Lua vm自带plugin管理和加载
 */

//typedef struct {
//    const char *name;
//    const void *ctx;
//} jplugin_be_t;

class module_private;
class module : public object
{
	J_DECLARE_PRIVATE(module);

public:
	module(const char *n);
	~module();

    static int regist(module *m);
    static int load();

    typedef int(*initializer)(...);
    void add_initializer(literal be, initializer i) {
        assert(initializers.count(be) == 0);
        initializers[be] = i;
    }

private:
    module *next;
    const char *name;
    const char *desciption;
    const char *licence;
    uint32_t version;
    //jelement_class_t **elements; // for load and *unload*
    //void **lelements;

    //virtual int init(...) = 0;
    //uint32_t inited : 1;
    std::map<literal, initializer> initializers;

    //jplugin_be_t **bes;
};

class J_EXTERN module_private : public object_private
{
	J_DECLARE_PUBLIC(module);

public:
	module_private() {}
	virtual ~module_private() {}
};

} // namespace jet


//int
//jplugin_load(const char *file, jplugin_t **plugin);
//int
//jplugin_load_be(const char *file);

//jelement_t *
//jplugin_create_element(const char *url);
//jelement_class_t *
//jplugin_get_element_class(const char *name);


