#pragma once
#include "jet.h"
#include "module.h"

namespace jet {

// business engine

class J_EXTERN be
{
public:
    be(const char *name, const char *desc = nullptr);
    ~be();

    //virtual int load_module(module *m) = 0;

    //static int load(const char *filename);
    //static int start(int *argc, char *argv[]);


//private:
    //void regist();
    virtual int start(int argc, char *argv[]) = 0;

    const char *name;
    const char *description;
    uint32_t version;
    be *next;
    //int (*pre_class_init)();
    //int (*post_class_init)();
    //int (*run)();
    //int (*load_plugin)(jplugin_t *p);

    void *ctx;

};

} // namespace jet