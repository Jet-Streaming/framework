#include "http.h"

namespace jet {
namespace http {

//void 
//http_generator_init(http_generator *generator, enum http_parser_type type)
//{}
//
//void 
//http_generator_settings_init(http_generator_settings *settings)
//{}

//static const jstr_t status_line_1xx[] = {
//    jstring("100 Continue"),
//    jstring("101 Switching Protocols"),
//    jstring("102 Processing"),              // RFC 2518, obsoleted by RFC 4918
//};
//
//static const jstr_t status_line_2xx[] = {
//    jstring("200 OK"),
//    jstring("201 Created"),
//    jstring("202 Accepted"),
//    jstring("203 Non - Authoritative Information"),
//    jstring("204 No Content"),
//    jstring("205 Reset Content"),
//    jstring("206 Partial Content"),
//    jstring("207 Multi-Status"),            // RFC 4918
//    jstring("208 Already Reported"),
//};
//
//static const jstr_t status_line_3xx[] = {
//    jstring("300 Multiple Choices"),
//    jstring("301 Moved Permanently"),
//    jstring("302 Found"),
//    jstring("303 See Other"),
//    jstring("304 Not Modified"),
//    jstring("305 Use Proxy"),
//    jstring_null,
//    jstring("307 Temporary Redirect"),
//    jstring("308 Permanent Redirect"),      // RFC 7238
//};
//
//static const jstr_t status_line_4xx[] = {
//    jstring("400 Bad Request"),
//    jstring("401 Unauthorized"),
//    jstring("402 Payment Required"),
//    jstring("403 Forbidden"),
//    jstring("404 Not Found"),
//    jstring("405 Method Not Allowed"),
//    jstring("406 Not Acceptable"),
//    jstring("407 Proxy Authentication Required"),
//    jstring("408 Request Time - out"),
//    jstring("409 Conflict"),
//    jstring("410 Gone"),
//    jstring("411 Length Required"),
//    jstring("412 Precondition Failed"),
//    jstring("413 Request Entity Too Large"),
//    jstring("414 Request - URI Too Large"),
//    jstring("415 Unsupported Media Type"),
//    jstring("416 Requested range not satisfiable"),
//    jstring("417 Expectation Failed"),
//    jstring("418 I\'m a teapot"),           // RFC 2324
//    jstring_null,
//    jstring_null,
//    jstring("421 Misdirected Request"),
//    jstring("422 Unprocessable Entity"),    // RFC 4918
//    jstring("423 Locked"),                  // RFC 4918
//    jstring("424 Failed Dependency"),       // RFC 4918
//    jstring("425 Unordered Collection"),    // RFC 4918
//};
//
//static const jstr_t status_line_5xx[] = {
//    jstring("500 Internal Server Error"),
//    jstring("501 Not Implemented"),
//    jstring("502 Bad Gateway"),
//    jstring("503 Service Unavailable"),
//    jstring("504 Gateway Time - out"),
//    jstring("505 HTTP Version not supported"),
//};
//
//struct status_lines_t {
//    jstr_t *st;
//    int n;
//};
//static const struct status_lines_t status_lines[] = {
//    { status_line_1xx, ARRAY_SIZE(status_line_1xx) },
//    { status_line_2xx, ARRAY_SIZE(status_line_2xx) },
//    { status_line_3xx, ARRAY_SIZE(status_line_3xx) },
//    { status_line_4xx, ARRAY_SIZE(status_line_4xx) },
//    { status_line_5xx, ARRAY_SIZE(status_line_5xx) },
//};
//
//// mbuf: Œ≤÷∏’Î
//int
//jhttp_resp_status(jmbuf_t **mbuf, int status, const char *status_line)
//{
//    int c;
//
//    jmbuf_litcpy(mbuf, "HTTP/1.1 ");
//
//    if (status_line) {
//        jmbuf_sprintf(mbuf, "%d ", status);
//    }
//    else {
//        c = status / 100 - 1;
//        if (c < 0 || c >= ARRAY_SIZE(status_lines)) {
//            return -EINVAL;
//        }
//        status -= (c + 1) * 100;
//        if (status < 0 || status >= status_lines[c].n) {
//            return -EINVAL;
//        }
//        status_line = status_lines[c].st[status].data;
//        assert(status_line);
//    }
//    jmbuf_sprintf(mbuf, "%s\r\n", status_line);
//
//    return EOK;
//}
//

char *
http_generator::append_packet(packet **p, uint32_t len)
{
    char *buf;
    int rc;

    buf = (*p)->append(len);
    if (buf) {
        return buf;
    }

    packet *p2 = new packet;
    rc = p2->alloc();
    if (rc) {
        goto err;
    }
    rc = (*p)->chain(p2);
    if (rc) {
        goto err;
    }
    buf = p2->append(len);
    if (!buf) {
        goto err;
    }

    *p = p2;
    return buf;

err:
    delete p2;
    return nullptr;
}


//> GET / HTTP/1.1

size_t 
http_generator::request_line(packet *pkt, enum http_method method,
    const char *uri, size_t urilen, int major, int minor)
{
    const char *methodname;
    size_t methodlen;
    char *buf;
    char version[5+5+2+1];
    int rc;

    // method
    methodname = http_method_str(method);
    methodlen = strlen(methodname);
    buf = append_packet(&pkt, methodlen);
    memcpy(buf, methodname, methodlen);
    buf = append_packet(&pkt, 1);
    *buf = ' ';

    // URI
    buf = append_packet(&pkt, urilen);
    memcpy(buf, uri, urilen);
    buf = append_packet(&pkt, 1);
    *buf = ' ';

    // version
    rc = snprintf(version, sizeof(version), "HTTP/%d.%d\r\n", major, minor);
    buf = append_packet(&pkt, rc);
    memcpy(buf, version, rc);

    return 0;
}

//< HTTP/1.1 200 OK

size_t 
http_generator::status_line(packet *pkt, int major, int minor, int code, const char *reason, size_t reasonlen)
{
    return 0;
}

//> User-Agent: curl/7.35.0
//> Host: cloudmedia.jiakai.me:8080
//> Accept: */*
//> 

//< Server: cloudmedia 1.2.2.3
//< Date: Thu, 28 Jul 2016 08:21:17 GMT
//< Content-Type: text/html
//< Content-Length: 442
//< Last-Modified: Wed, 08 Jun 2016 06:07:24 GMT
//< Connection: keep-alive
//< ETag: "5757b61c-1ba"
//< Accept-Ranges: bytes
//<

size_t 
http_generator::header(packet *pkt, const char *k, size_t kl, const char *v, size_t vl)
{
    char *buf;

    // key
    buf = append_packet(&pkt, kl);
    memcpy(buf, k, kl);

    buf = append_packet(&pkt, 2);
    *buf++ = ':';
    *buf++ = ' ';

    // value
    buf = append_packet(&pkt, vl);
    memcpy(buf, v, vl);

    buf = append_packet(&pkt, 2);
    *buf++ = '\r';
    *buf++ = '\n';

    return 0;
}

int 
http_generator::headers_done(packet *pkt)
{
    char *buf;
    buf = append_packet(&pkt, 2);
    *buf++ = '\r';
    *buf++ = '\n';
    return EOK;
}


} // http
} // jet