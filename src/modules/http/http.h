#pragma once
#include <jet/jet.h>
//#include "module.h"
#include <jet/pad.h>
#include <jet/element.h>
#include "http_parser.h"
#include "http_generator.h"

namespace jet {
namespace http {

class http : public element
{
public:
    http(http_parser_settings *settings);
    ~http();

    //void parser_init(enum http_parser_type type, http_parser_settings *settings);
    size_t parse(const char *data, size_t len);

    size_t gen_request_line(packet *pkt, enum http_method method, const char *uri, size_t urilen, int major, int minor) {
        return generator.request_line(pkt, method, uri, urilen, major, minor);
    }
    size_t gen_header(packet *pkt, const char *k, size_t kl, const char *v, size_t vl) {
        return generator.header(pkt, k, kl, v, vl);
    }
    int gen_headers_done(packet *pkt) {
        return generator.headers_done(pkt);
    }

    int request(packet *pkt, void *data);

private:
    static int on_mux_push(pad *pad, pad::push_req *req, pad::push_type type);
    static int on_body_push(pad *pad, pad::push_req *req, pad::push_type type);

    static int on_http_body(http_parser *p, const char *at, size_t length);

protected:
    //pad::push_req pushin;
    //pad::push_req pushout;
    http_parser parser;
    http_parser_settings *parser_setting;
	http_generator generator;

    pad mux_pad;
    pad::push_req mux_pushout;

    pad body_pad;
    pad::push_req body_pushout;
};

} // http
} // jet