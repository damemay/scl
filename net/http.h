#ifndef __SCL_HTTP_H__
#define __SCL_HTTP_H__
#include "socket.h"
#include "../data/map.h"

#define SCL_HTTP_NEWLINE	    "\r\n"
#define SCL_HTTP_BREAK	    	    "\r\n\r\n"
#define SCL_HTTP_URL_PREFIX_HTTP    "http://"
#define SCL_HTTP_URL_PREFIX_HTTPS   "https://"
#define SCL_HTTP_PORT_SIZE_LIMIT    32
#define SCL_HTTP_NAME_SIZE_LIMIT    1024
#define SCL_HTTP_QUERY_SIZE_LIMIT   4096

typedef enum scl_http_request_method {
    scl_http_request_get,
    scl_http_request_head,
    scl_http_request_post,
} scl_http_request_method;

typedef struct scl_http_request {
    scl_http_request_method method;
    char* url;
    int timeout;
    int follow_redirects;
    void* data;
    char* basic_auth[2];
    scl_map* headers;
    scl_map* params;
} scl_http_request;

typedef struct scl_http_response {
    char* url;
    char* text;
    void* data;
    int status_code;
    scl_map* headers;
} scl_http_response;

scl_http_response* scl_http_request_perform(scl_http_request* request);

#endif
