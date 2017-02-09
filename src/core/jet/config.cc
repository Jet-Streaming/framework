
#include "config.h"

namespace jet {

config::config()
{
}

config::~config()
{
}

} // namespace jet

//static jconf_t jconf;
//
//int
//jconf_init()
//{
//    jpad_seginfo_t *si;
//
//    // OS TCP/IP stack
//    si = &jconf.def_seginfo;
//    si->maxlen = 2048*2 - sizeof(jmbuf_t);
//    si->minlen = si->maxlen;
//    si->headroom = 0;
//    si->contiguous = 0;
//    si->valid = 1;
//
//    return EOK;
//}
//
//jconf_t *
//jconf_data()
//{
//    return &jconf;
//}