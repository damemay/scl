#ifndef __SCL_HTTP_H__
#define __SCL_HTTP_H__
#include "socket.h"
#include "../data/map.h"

typedef struct scl_http_request {
    char* url;
    char* method;
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

scl_http_response* scl_http_perform_request(scl_http_request* request);

#endif
