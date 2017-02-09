#include "http.h"

namespace jet {
namespace http {


http::http(http_parser_settings *settings)
    : mux_pad("mux", this, on_mux_push)
    , body_pad("body", this, on_body_push)
    , parser_setting(settings)
{
    int rc;
    const rt_class *rtc;

    http_parser_init(&parser, HTTP_REQUEST);

    assert(!parser_setting->on_body);
    parser_setting->on_body = on_http_body;

    rtc = rt_class::get("sequence");
    assert(rtc);
    rc = mux_pad.set_cap(pad::direction::DIN, rtc);
    assert(EOK == rc);
    rc = mux_pad.set_cap(pad::direction::DOUT, "stream");
    assert(EOK == rc);
    rc = body_pad.set_cap(pad::direction::DIN, rtc);
    assert(EOK == rc);
    rc = body_pad.set_cap(pad::direction::DOUT, rtc);
    assert(EOK == rc);
}

http::~http()
{}

//void 
//http::parser_init(enum http_parser_type type, http_parser_settings *settings)
//{
//    http_parser_init(&parser, );
//    parser_setting = settings;
//}

size_t 
http::parse(const char *data, size_t len)
{
    return http_parser_execute(&parser, parser_setting, data, len);
}

int 
http::request(packet *pkt, void *data)
{
    int rc;

	parser.data = data;
    http_parser_init(&parser, HTTP_RESPONSE);
    rc = mux_pad.push(&mux_pushout, pkt);

    return EOK;
}

int
http::on_mux_push(pad *pad, pad::push_req *req, pad::push_type type)
{
    size_t rc;
    http *self;

	if (pad::push_type::PACKET != type) {
		return EOK;
	}

    self = container_of(pad, http, mux_pad);
    rc = http_parser_execute(&self->parser, self->parser_setting,
        (char *)req->pkt->data(), req->pkt->len());
    //jmbuf_free(buf);
	
    if (rc == req->pkt->len()) {
        pad::ack_push(req);
    } else {
        if (self->parser.upgrade) {
            ;
        }
        else {
            self->parser_setting->on_error(&self->parser);
        }
    }
    return EOK;
}

int
http::on_body_push(pad *pad, pad::push_req *req, pad::push_type type)
{
    return EOK;
}

int
http::on_http_body(http_parser *p, const char *at, size_t length)
{
    http *self;
    int rc;

    self = static_cast<http *>(p->data);
    printf("%s: %d\n", __FUNCTION__, length);

    //self->body_pushout.pkt = nullptr;
    //rc = self->body_pad.push(&self->body_pushout);

    return 0;
}


} // http
} // jet

//int
//jhttp_resp_header(jmbuf_t **mbuf, const char *k, const char *v)
//{
//    if (k) {
//        assert(v);
//        jmbuf_sprintf(mbuf, "%s: %s\r\n", k, v);
//    }
//    else {
//        jmbuf_litcpy(mbuf, "\r\n");
//    }
//
//    return EOK;
//}
//
//
////static void 
////response_cb(jmbuf_t *mbuf, int status)
////{}
////
////int
////jhttp_send(jhttp_t *self, jmbuf_t *mbuf)
////{
////    int rc;
////
////    rc = jpad_push(&self->http_pad, mbuf, response_cb);
////    return rc;
////}
//
//static int
//inst_init(jhttp_t *self, va_list args)
//{
//    int rc;
//    jelement_t *elm;
//
//    jobject_init(jpad_class(), &self->http_pad);
//    strncpy(self->http_pad.name, "http", sizeof(self->http_pad.name));
//    http_parser_init(&self->parser, HTTP_REQUEST);
//
//    elm = (jelement_t *)self;
//    rc = jelement_add_pad(elm, &self->http_pad);
//    self->http_pad.on_push = on_http_push;
//
//    //http_parser_settings_init(&self->parser_setting);
//    //jlua_http_init(self);
//
//    return rc;
//}
//
///*static jproperty_t props[] = {
//    {
//        .name = "listen",
//        .description = "start listening on a TCP port",
//        .unwritable = 1,
//        .type = PROP_METHOD,
////        .method = {
////            .type = "",
////            .entry = _listen,
////        },
//    },
//};*/
//
//static void
//vtbl_init(jhttp_class_t *self)
//{
//    jelement_class_t *elm;
//    jobject_class_t *obj;
//
//    obj = (jobject_class_t *)self;
//
//    elm = (jelement_class_t *)self;
//    //elm->inst_size = sizeof(jhttp_t);
//    elm->name = "http";
//
//    //jlua_http_class_init(self);
//}
//
//J_DEFINE_CLASS(jhttp, jelement, inst_init, vtbl_init);
