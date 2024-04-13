#include "tinysockets.h"

int send_all(int fd, const void* buf, const uint32_t size) {
    size_t total = 0, left = size, ret = 0;
    while(total < size) {
        ret = send(fd, buf+total, left, 0);
        if(ret == -1) return -1;
        total += ret;
        left -= ret;
    }
    return total;
}

int recv_all(int fd, void* buf, const uint32_t size) {
    size_t total = 0, left = size, ret = 0;
    while(total < size) {
        ret = recv(fd, buf+total, left, 0);
        if(ret == -1) return -1;
        total += ret;
        left -= ret;
    }
    return total;
}

static void* get_in_addr(struct sockaddr* sa) {
    if(sa->sa_family == AF_INET) return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

static struct addrinfo* init_addrinfo(const char* host, const char* port,
	const struct addrinfo* hints) {
    int res;
    struct addrinfo* info;
    if((res = getaddrinfo(host, port, hints, &info)) != 0) return NULL;
    return info;
}

int tinyclient_conn(tinyclient* client) {
    if(!client) return -1;
        struct addrinfo hints = {
        .ai_family      = AF_UNSPEC,
        .ai_socktype    = SOCK_STREAM,
    };
    struct addrinfo* info = init_addrinfo(client->host, client->port, &hints);
    for(client->ai = info; client->ai != NULL; client->ai = client->ai->ai_next) {
        if((client->fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) == -1)
            continue;
        if(connect(client->fd, info->ai_addr, info->ai_addrlen) == -1) {
            close(client->fd);
            continue;
        }
        break;
    }
    freeaddrinfo(info);
    return 1;
}

void tinyclient_free(tinyclient* client) {
    freeaddrinfo(client->ai);
}
