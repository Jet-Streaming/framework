#pragma once
//#include "http.h"
#include "http_parser.h"
#include <jet/packet.h>

namespace jet {
namespace http {

class http_generator {
    /** PRIVATE **/
    unsigned int type : 2;         /* enum http_parser_type */
    unsigned int flags : 7;        /* F_* values from 'flags' enum; semi-public */
    unsigned int state : 7;        /* enum state from http_parser.c */
    unsigned int header_state : 8; /* enum header_state from http_parser.c */
    unsigned int index : 8;        /* index into current matcher */

    //uint32_t nread;          /* # bytes read in various scenarios */
    //uint64_t content_length; /* # bytes in body (0 if no Content-Length header) */

                                /** READ-ONLY **/
    //unsigned short http_major;
    //unsigned short http_minor;
    unsigned int status_code : 16; /* responses only */
    //unsigned int method : 8;       /* requests only */
    //unsigned int http_errno : 7;

    /* 1 = Upgrade header was present and the parser has exited because of that.
    * 0 = No upgrade header present.
    * Should be checked when http_parser_execute() returns in addition to
    * error checking.
    */
    unsigned int upgrade : 1;

    /** PUBLIC **/
    void *data; /* A pointer to get hook to the "connection" or "socket" object */

    char *append_packet(packet **p, uint32_t len);

public:
    size_t request_line(packet *pkt, enum http_method method, const char *uri, size_t urilen, int major, int minor);
    size_t status_line(packet *pkt, int major, int minor, int code, const char *reason, size_t reasonlen);
    size_t header(packet *pkt, const char *k, size_t kl, const char *v, size_t vl);
    int headers_done(packet *pkt);
};


//struct http_generator_settings {
//    http_cb      on_message_begin;
//    http_data_cb on_url;
//    http_cb      on_url_complete;
//    http_data_cb on_status;
//    http_cb      on_status_complete;
//    http_data_cb get_header_field;
//    http_data_cb get_header_value;
//    http_cb      get_header_complete;
//    http_cb      get_headers_complete;
//    //http_data_cb on_body;
//    http_cb      on_message_complete;
//    /* When on_chunk_header is called, the current chunk length is stored
//    * in parser->content_length.
//    */
//    http_cb      on_chunk_header;
//    http_cb      on_chunk_complete;
//
//    int(*alloc_buffer)(http_generator *gen, char **buf, size_t *size);
//};
//
//typedef enum http_parser_type http_generator_type;
//void http_generator_init(http_generator *generator, http_generator_type type);


/* Initialize http_parser_settings members to 0
*/
//void http_generator_settings_init(http_generator_settings *settings);


/* Executes the parser. Returns number of parsed bytes. Sets
* `parser->http_errno` on error. */
//size_t http_generator_execute(http_generator *generator,
//    const http_generator_settings *settings);

} // http
} // jet