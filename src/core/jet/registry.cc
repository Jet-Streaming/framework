/*
 * registry.c
 *
 *  Created on: Jul 5, 2015
 *      Author: root
 */
#include "registry.h"


typedef struct {
    const char *element;
    const char *plugin;
} reg_t;

#ifdef _DEBUG
#define DIR "Debug"
#else
#define DIR "Release"
#endif

static reg_t regs[] = {
    { "ostcp",  "../win64/"DIR"/coreplugin.dll" },
    { "rtmp", "../win64/"DIR"/rtmp.dll" },
    { "rtmcsp", "../win64/"DIR"/rtmcsp.dll" },
    { "http", "../win64/"DIR"/http.dll" },
};

//#ifdef _WIN32
//rc = jplugin_load("Debug/coreplugin.dll");
//#else
//rc = jplugin_load("out/Debug/lib.target/libcoreplugin.so");
//#endif
//assert(EOK == rc);
//#ifdef _WIN32
//rc = jplugin_load("Debug/http.dll");
//rc = jplugin_load("Debug/rtmp.dll");
//#else
//rc = jplugin_load("out/Debug/lib.target/libhttp.so");
//#endif

const char *
jreg_get_plugin(const char *element)
{
    int i;

    for (i = 0; i != ARRAY_SIZE(regs); ++i) {
        if (!strcmp(element, regs[i].element)) {
            return regs[i].plugin;
        }
    }
    return NULL;
}
