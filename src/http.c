#include "../net/http.h"
#include "../net/socket.h"
#include "../utils/helpers.h"
#include <string.h>
#include <ctype.h>
#include <poll.h>
#include <fcntl.h>

static int init_request(scl_http_request* request) {
    if(!request->url) return -1;
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
    if(getaddrinfo(hostname, NULL, NULL, &info) != 0) return -1;
    inet_ntop(info->ai_family, get_in_addr(info->ai_addr), ip, SCL_HTTP_NAME_SIZE_LIMIT);
    freeaddrinfo(info);
    return 0;
}

static int parse_url(char* url, char* hostname, char* ip, char* port, char* query) {
    int has_prefix = get_port(url, port);
    char* test_url = has_prefix ? strstr(url, "//")+2 : url;
    get_hostname(test_url, hostname);
    get_query(test_url, query);
    if(get_ip(hostname, ip) == -1) return -1;
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
    if(poll(pfd, 1, timeout*1000) == 0) return -1;
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
    res = sprintf(buffer, "host: %s" SCL_HTTP_NEWLINE, host);
    size += res, buffer += res;
    res = sprintf(buffer, "user-agent: " SCL_HTTP_USER_AGENT SCL_HTTP_NEWLINE);
    size += res, buffer += res;
    if(r->basic_auth[0]) {}
    if(r->headers) {}
    if(r->method != scl_http_request_head && r->data) {}
    res += sprintf(buffer, SCL_HTTP_NEWLINE);
    size += res, buffer += res;
    if(!poll_event(fd, r->timeout, POLLOUT)) return -1;
    if(scl_send(fd, message, size) <= 0) return -1;
    SCL_VLOG("\n%s", message);
    return 0;
}

static int recv_terminator(int fd, char* buffer, char* terminator, int timeout) {
    size_t b_off = 0;
    uint16_t off = SCL_HTTP_QUERY_SIZE_LIMIT;
    fcntl(fd, F_SETFL, O_NONBLOCK);
    do {
	if(poll_event(fd, timeout, POLLIN)) {
	    int r = scl_recv(fd, buffer+b_off, off);
	    b_off += off;
	}
    } while(!strstr(buffer, terminator));
    buffer[b_off] = '\0';
    return 0;
}

static int read_response(scl_http_response* r, int fd, int timeout) {
    char response[SCL_HTTP_MESSAGE_SIZE_LIMIT];
    if(recv_terminator(fd, response, SCL_HTTP_TERMINATOR, timeout) == -1) return -1;
    SCL_VLOG("\n%s", response);
    return 0;
}

static int perform(scl_http_request* request, scl_http_response* response, int fd, char* host, char* query) {
    if(send_request(request, fd, host, query) == -1) return -1;
    if(read_response(response, fd, request->timeout) == -1) return -1;
    return 0;
}

int scl_http_request_perform(scl_http_request* request, scl_http_response* response) {
    if(init_request(request) == -1) return -1;
    char hostname[SCL_HTTP_NAME_SIZE_LIMIT];
    char ip[SCL_HTTP_NAME_SIZE_LIMIT];
    char port[SCL_HTTP_PORT_SIZE_LIMIT];
    char query[SCL_HTTP_QUERY_SIZE_LIMIT];
    parse_url(request->url, hostname, ip, port, query);
    SCL_VLOG("%s %s %s %s", hostname, ip, port, query);
    scl_socket_client sock = {
	.host = ip,
	.port = port,
    };
    if(scl_socket_client_init(&sock) == -1) return -1;
    if(request->params) {}
    perform(request, response, sock.fd, hostname, query);

    return 0;
}
