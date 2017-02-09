/*
 * tcp.h
 *
 *  Created on: Nov 11, 2015
 *      Author: root
 */

#pragma once
//#undef BUILDING_UV_SHARED
//#define USING_UV_SHARED 1
#include <jet/jet.h>
#include "module.h"
#include <jet/pad.h>
#include <jet/element.h>

//typedef void(*jtcp_send_cb)(jtcp_t* client, int status);

//typedef struct {
//    jtcp_listen_cb connection;
//} jtcp_callback_t;

namespace jet {
namespace coremodule {

class tcp : public element
{
public:
    tcp();
    ~tcp();

    typedef void(*listen_cb)(tcp* server, int status);
    typedef void(*connect_cb)(tcp* client, int status);

    //virtual int bind(const char *ip, uint16_t port) = 0;
    //virtual int listen(int backlog, listen_cb cb) = 0;
    //virtual int accept(tcp *client) = 0;
    //virtual int connect(const char *ip, uint16_t port, connect_cb cb) = 0;

	//virtual const char *impl_name() = 0;
	//typedef tcp *(*impl_allocator)();
	//static void impl_register(const char *name, impl_allocator a);
	//static tcp *impl_new(const char *name);

protected:
    //pad::push_req pushin;
    pad::push_req pushout;
};

} // coremodule
} // jet