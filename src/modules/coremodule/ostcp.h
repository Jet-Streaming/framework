#pragma once
#include "tcp.h"
#include <jet/jet.h>

namespace jet {
namespace coremodule {

class ostcp : public tcp
{
public:
    ostcp();
    ~ostcp();

    int bind(const char *ip, uint16_t port);
    int listen(int backlog, listen_cb cb);
    int accept(tcp *client);
    int connect(const char *ip, uint16_t port, connect_cb cb);

	//const char *klass() {
	//	return "ostcp";
	//}

private:
    //friend class tcp;
    pad data_pad;
	uv_tcp_t uvtcp;
    union {
        uv_write_t wreq;
        uv_connect_t connreq;
    };

    static void on_listen(uv_stream_t* server, int status);
    static void on_connect(uv_connect_t* req, int status);
    static void on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
    static void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

    static void write_cb(uv_write_t* wreq, int status);    
    static int on_push(pad *pad, pad::push_req *req, pad::push_type type);
    //static void on_push_event(pad *pad, pad::push_req *req, pad::push_event event);
    //static int push_cb(pad *pad, pad::push_req *req);

    int on_packet(pad::push_req *req);
    int on_ack(pad::push_req *req);
    int on_cancel(pad::push_req *req);
    int on_unlink(pad::push_req *req);

};

} // coremodule
} // jet