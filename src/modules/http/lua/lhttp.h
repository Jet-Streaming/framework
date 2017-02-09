#pragma once
#include "../http.h"
#include <lua/jlelement.h>
//#include <memory>
#include <string>

namespace jet {
namespace http {

//class string {
//	char s[1024];
//	size_t l;
//
//public:
//	string() 
//		: l(0)
//	{}
//	void cl
//};


//typedef struct {
//
//    //unsigned header_sent: 1;
//    //unsigned major_version : 2;
//    //unsigned minor_version : 2;
//
//    //char uri[1024];
//    //size_t urilen;
//
//    //lhttp_state_t state;
//    std::string k, v;
//
//    //int thread;
//    //int req_header_table;
//    //int res_header_table;
//    //int uri_args_table;
//    //int table;
//} req;

class lhttp : public http, public lua::lelement
{
public:
    lhttp();
    ~lhttp();

private:
	static int on_message_begin(http_parser *p);
    static int on_status(http_parser *p, const char *at, size_t length);
    static int on_status_complete(http_parser *p);
    static int on_url(http_parser *p, const char *at, size_t length);
	static int on_url_complete(http_parser *p);
	static int on_header_field(http_parser *p, const char *at, size_t length);
	static int on_header_value(http_parser *p, const char *at, size_t length);
	static int on_header_complete(http_parser *p);

private:
    static http_parser_settings pss;
	//req parsing_req;
	std::string parser_k;
	std::string parser_v;
};


} // http
} // jet