#pragma once
#include "../tcp.h"
#include <lua/jlelement.h>

namespace jet {
namespace coremodule {

//class ltcp : public tcp, public lua::lelement
//{
//public:
//    ltcp ();
//    ~ltcp ();
//
//private:
//
//};

} // coremodule
} // jet

#if 0
typedef enum {
    LTCP_S_NONE,
    LTCP_S_CONNECTING,
    LTCP_S_READING,
    LTCP_S_WRITING,
} jltcp_state_t;

struct jltcp_s {
    jlelement_t parent;
    //int on_connect;

    //int blocking_thread;
    jltcp_state_t state;
    int ref;

    //lua_State *L;
};

struct jltcp_class_s {
    jlelement_class_t parent;
};
#endif // _LTCP_H