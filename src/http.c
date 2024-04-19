#include "../net/http.h"
#include "../net/socket.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <poll.h>
#include <fcntl.h>

void scl_http_error_parse(scl_http_error error, char* buffer, size_t size) {
    switch(error) {
	case scl_http_error_unknown: snprintf(buffer, size, "unknown"); break;
	case scl_http_error_bad_url: snprintf(buffer, size, "bad url"); break;
	case scl_http_error_getaddrinfo: snprintf(buffer, size, "getaddrinfo"); break;
	case scl_http_error_polling: snprintf(buffer, size, "polling"); break;
	case scl_http_error_send: snprintf(buffer, size, "send"); break;
	case scl_http_error_recv: snprintf(buffer, size, "recv"); break;
	case scl_http_error_sock_closed: snprintf(buffer, size, "sock closed"); break;
	case scl_http_error_str_not_found: snprintf(buffer, size, "str not found"); break;
    }
}

static int init_request(scl_http_request* request) {
    if(!request->url) return scl_http_error_bad_url;
    if(request->timeout == 0) request->timeout = 300;
    return 0;
}

static int get_port(char* url, char* port) {
    int has_prefix = 0;
    if(strstr(url, SCL_HTTP_URL_PREFIX_HTTPS)) {
	strcpy(port, "443");
	has_prefix = 1;
    } else if(strstr(url, SCL_HTTP_URL_PREFIX_HTTP)) {
	strcpy(port, "80");
	has_prefix = 1;
    } else {
	char* ptr;
	if((ptr = strchr(url, ':'))) {
	    int i = 0;
	    while(isdigit((ptr+1)[i])) i++;
	    strncpy(port, (ptr+1), i),
		*(port+i) = '\0';
	} else strcpy(port, "80");
    }
    return has_prefix;
}

static void get_hostname(char* no_prefix_url, char* hostname) {
    char* special;
    if((special = strchr(no_prefix_url, ':')))
	strncpy(hostname, no_prefix_url, special-no_prefix_url),
	    *(hostname+(special-no_prefix_url)) = '\0';
    else if((special = strchr(no_prefix_url, '/')))
	strncpy(hostname, no_prefix_url, special-no_prefix_url),
	    *(hostname+(special-no_prefix_url)) = '\0';
    else strcpy(hostname, no_prefix_url);
}

static void get_query(char* no_prefix_url, char* query) {
    char* first_slash;
    if((first_slash = strchr(no_prefix_url, '/'))) strcpy(query, first_slash);
    else strcpy(query, "/");
}

static void* get_in_addr(struct sockaddr* sa) {
    if(sa->sa_family == AF_INET) return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

static int get_ip(char* hostname, char* ip) {
    struct addrinfo* info;
    if(getaddrinfo(hostname, NULL, NULL, &info) != 0) return scl_http_error_getaddrinfo;
    inet_ntop(info->ai_family, get_in_addr(info->ai_addr), ip, SCL_HTTP_NAME_SIZE_LIMIT);
    freeaddrinfo(info);
    return 0;
}

static int parse_url(char* url, char* hostname, char* ip, char* port, char* query) {
    int has_prefix = get_port(url, port);
    char* test_url = has_prefix ? strstr(url, "//")+2 : url;
    get_hostname(test_url, hostname);
    get_query(test_url, query);
    int res = 0;
    if((res = get_ip(hostname, ip)) < 0) return res;
    return 0;
}

static void parse_method(scl_http_request_method method, char* str) {
    switch(method) {
	case scl_http_request_get: strcpy(str, "GET"); break;
	case scl_http_request_head: strcpy(str, "HEAD"); break;
	case scl_http_request_post: strcpy(str, "POST"); break;
    }
}

static int poll_event(int fd, int timeout, short event) {
    struct pollfd pfd[1];
    pfd[0].fd = fd;
    pfd[0].events = event;
    if(poll(pfd, 1, timeout*1000) == 0) return scl_http_error_polling;
    return pfd[0].revents & event;
}

static int send_request(scl_http_request* r, int fd, char* host, char* query) {
    size_t reserved_size = SCL_HTTP_MESSAGE_SIZE_LIMIT;
    if(r->headers) reserved_size += sizeof(r->headers->data);
    if(r->data) reserved_size += r->data_size;
    char message[reserved_size];
    char method[SCL_HTTP_METHOD_SIZE_LIMIT];
    parse_method(r->method, method);
    char* buffer = message;
    uint64_t size = 0;
    int res = sprintf(buffer, "%s %s HTTP/1.1" SCL_HTTP_NEWLINE, method, query);
    size += res, buffer += res;
    res = sprintf(buffer, "Host: %s" SCL_HTTP_NEWLINE, host);
    size += res, buffer += res;
    res = sprintf(buffer, "User-Agent: " SCL_HTTP_USER_AGENT SCL_HTTP_NEWLINE);
    size += res, buffer += res;
    if(r->basic_auth[0]) {}
    if(r->headers) {}
    if(r->method != scl_http_request_head && r->data) {}
    res += sprintf(buffer, SCL_HTTP_NEWLINE);
    size += res, buffer += res;
    if(!poll_event(fd, r->timeout, POLLOUT)) return scl_http_error_polling;
    if(scl_send(fd, message, size) <= 0) return scl_http_error_send;
    return 0;
}

static int recv_terminator(int fd, char* buffer, char* terminator, uint16_t off, int timeout) {
    size_t b_off = 0;
    fcntl(fd, F_SETFL, O_NONBLOCK);
    do {
	if(poll_event(fd, timeout, POLLIN)) {
	    if(scl_recv(fd, buffer+b_off, off)==0) return scl_http_error_sock_closed;
	    b_off += off;
	}
    } while(!strstr(buffer, terminator));
    buffer[b_off] = '\0';
    const int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags^O_NONBLOCK);
    return 0;
}

static int add_header(scl_http_response* r, const char* line, size_t len, const char* separator) {
    size_t key_size = separator-line;
    size_t val_size = len-(separator-line);
    char key[key_size];
    char val[val_size];
    sprintf(key, "%.*s", (int)key_size, line);
    sprintf(val, "%.*s", (int)val_size, separator+2);
    return scl_map_add(r->headers, key, val, val_size);
}

static void get_status_code_from_line(scl_http_response* r, char* line) {
    char* token = strtok(line, " ");
    while(token) {
	if(isdigit(token[0])) {
	    r->status_code = atoi(token);
	    return;
	}
	token = strtok(NULL, " ");
    }
}

static int parse_headers(scl_http_response* r, char* response) {
    char* nl = response, *next_nl = response;
    r->headers = scl_map_create(10);
    if(!r->headers) return -1;
    while(1) {
	next_nl = strstr(nl, SCL_HTTP_NEWLINE);
	size_t len = next_nl-nl;
	if(len == 0) break;
	char line[len+1];
	sprintf(line, "%.*s", (int)(len), nl);
	char* separator = strchr(line, ':');
	if(separator) {
	    if(add_header(r, line, len, separator) == -1) return -1;
	} else get_status_code_from_line(r, line);
	nl = next_nl+2;
    }
    return 0;
}

static int data_with_content_length(scl_http_response* r, int fd, int timeout, char* size, char* response) {
    long content_length = atoi(size);
    r->data = malloc(content_length+1);
    if(!r->data) return -1;
    r->data_size = content_length+1;
    char* data = strstr(response, SCL_HTTP_TERMINATOR)+4;
    strcpy(r->data, data);
    int remaining = content_length - strlen(data);
    if(remaining <= 0) return 0;
    if(poll_event(fd, timeout, POLLIN))
	if(scl_recv(fd, r->data+strlen(data), remaining) == 0)
	    return scl_http_error_sock_closed;
    return 0;
}

static int dyn_recv_terminator(scl_http_response* r, int fd, char* terminator, uint16_t off, int timeout) {
    size_t pos = 0;
    fcntl(fd, F_SETFL, O_NONBLOCK);
    int i=0;
    while(1) {
	if(poll_event(fd, timeout, POLLIN)) {
	    if(pos+off > r->data_size) {
	    	void* tmp = realloc(r->data, r->data_size*2);
	    	if(!tmp) return -1;
	    	r->data = tmp;
	    	r->data_size *= 2;
	    }
	    int recvd = recv(fd, r->data+pos, off, 0);
	    if(recvd == 0) return scl_http_error_sock_closed;
	    else if(recvd < 0) recvd = off;
	    char* point = NULL;
	    if((point = strstr(r->data+pos, SCL_HTTP_TERMINATOR))) {
		pos += recvd;
	        break;
	    }
	    pos += recvd;
	}
    }
    ((char*)(r->data))[pos] = '\0';
    const int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags^O_NONBLOCK);
    return 0;
}

static int concat_parse(scl_http_response* r, char* response) {
    char* start = strstr(response, SCL_HTTP_TERMINATOR);
    sprintf(r->data, "%s%s", start, (char*)r->data);
    size_t size = strlen(r->data);
    char* tmp = malloc(size);
    if(!tmp) return -1;
    char* t = r->data+4, *chunk_end = NULL, *next_t = NULL;
    while(1) {
	if(strtoul(t, &chunk_end, 16)) {
	    next_t = strstr(chunk_end+2, SCL_HTTP_NEWLINE);
	    chunk_end += 2;
	    size_t len = next_t-chunk_end;
	    if(len==0) break;
	    char line[len+1];
	    sprintf(line, "%.*s", (int)len, chunk_end);
	    strcat(tmp, line);
	    t = next_t+2;
	    if(strtoul(t, NULL, 10) == 0) break;
	}
    }
    free(r->data);
    r->data = tmp;
    r->data_size = size;
    return 0;
}

static int read_chunks(scl_http_response* r, int fd, int timeout, char* response) {
    int ret = 0;
    r->data_size = SCL_HTTP_MESSAGE_SIZE_LIMIT*2;
    r->data = malloc(r->data_size);
    if(!r->data) return -1;
    if((ret = dyn_recv_terminator(r, fd, SCL_HTTP_CHUNK_TERMINATOR, SCL_HTTP_QUERY_SIZE_LIMIT, timeout)) < 0) return ret;
    concat_parse(r, response);
    return 0;
}

static int read_response(scl_http_response* r, int fd, scl_http_request* req) {
    char response[SCL_HTTP_MESSAGE_SIZE_LIMIT];
    int ret = 0;
    if((ret = recv_terminator(fd, response, SCL_HTTP_TERMINATOR, SCL_HTTP_QUERY_SIZE_LIMIT, req->timeout)) < 0) return ret;
    if((ret = parse_headers(r, response) < 0)) return ret;
    if(req->method != scl_http_request_head) {
	char* value = NULL;
	if((value = scl_map_get(r->headers, "Transfer-Encoding"))) {
	    if((ret = read_chunks(r, fd, req->timeout, response)) < 0)
    		return ret;
	} else if((value = scl_map_get(r->headers, "Content-Length"))) {
	    if((ret = data_with_content_length(r, fd, req->timeout, value, response)) < 0)
		return ret;
	}
    }
    return 0;
}

static int perform(scl_http_request* request, scl_http_response* response, int fd, char* host, char* query) {
    int ret = 0;
    if((ret = send_request(request, fd, host, query)) < 0) return ret;
    if((ret = read_response(response, fd, request)) < 0) return ret;
    return 0;
}

int scl_http_request_perform(scl_http_request* request, scl_http_response* response) {
    int ret = 0;
    if((ret = init_request(request)) < 0) return ret;
    char hostname[SCL_HTTP_NAME_SIZE_LIMIT];
    char ip[SCL_HTTP_NAME_SIZE_LIMIT];
    char port[SCL_HTTP_PORT_SIZE_LIMIT];
    char query[SCL_HTTP_QUERY_SIZE_LIMIT];
    parse_url(request->url, hostname, ip, port, query);
    scl_socket_client sock = {
	.host = ip,
	.port = port,
    };
    if(scl_socket_client_init(&sock) == -1) return -1;
    if(request->params) {}
    response->data = NULL;
    response->data_size = 0;
    response->headers = NULL;
    response->status_code = 0;
    if((ret = perform(request, response, sock.fd, hostname, query)) < 0) return ret;
    return 0;
}
