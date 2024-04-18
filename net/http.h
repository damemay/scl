#ifndef __SCL_HTTP_H__
#define __SCL_HTTP_H__
#include "socket.h"
#include "../data/map.h"

#define SCL_HTTP_USER_AGENT	    "scl-http-request"
#define SCL_HTTP_NEWLINE	    "\r\n"
#define SCL_HTTP_TERMINATOR	    "\r\n\r\n"
#define SCL_HTTP_CHUNK_TERMINATOR   "0\r\n\r\n"
#define SCL_HTTP_URL_PREFIX_HTTP    "http://"
#define SCL_HTTP_URL_PREFIX_HTTPS   "https://"
#define SCL_HTTP_PORT_SIZE_LIMIT    32
#define SCL_HTTP_NAME_SIZE_LIMIT    1024
#define SCL_HTTP_QUERY_SIZE_LIMIT   1024
#define SCL_HTTP_METHOD_SIZE_LIMIT  5
#define SCL_HTTP_MESSAGE_SIZE_LIMIT 5120

typedef enum scl_http_error {
    scl_http_error_unknown = -1,
    scl_http_error_bad_url = -2,
    scl_http_error_getaddrinfo = -3,
    scl_http_error_polling = -4,
    scl_http_error_send = -5,
    scl_http_error_recv = -6,
    scl_http_error_sock_closed = -7,
} scl_http_error;

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
    size_t data_size;
    char* basic_auth[2];
    scl_map* headers;
    scl_map* params;
} scl_http_request;

typedef struct scl_http_response {
    void* data;
    uint16_t status_code;
    scl_map* headers;
} scl_http_response;

int scl_http_request_perform(scl_http_request* request, scl_http_response* response);
void scl_http_error_parse(scl_http_error error, char* buffer, size_t size);

#endif
