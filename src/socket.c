#include "../net/socket.h"
#include "../utils/helpers.h"
#include <netdb.h>
#include <stdint.h>
#include <stdlib.h>

int scl_send(int fd, const void* buf, const uint32_t size) {
    size_t total = 0, left = size, ret = 0;
    while(total < size) {
        ret = send(fd, buf+total, left, 0);
        if(ret == -1) return -1;
        total += ret;
        left -= ret;
    }
    return total;
}

int scl_recv(int fd, void* buf, const uint32_t size) {
    size_t total = 0, left = size, ret = 0;
    while(total < size) {
        ret = recv(fd, buf+total, left, 0);
        if(ret == -1) return -1;
        total += ret;
        left -= ret;
    }
    return total;
}

static void sigchld_handler(int s) {
    int t_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = t_errno;
}

static int kill_dead() {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1) return -1;
    return 0;
}

static void* get_in_addr(struct sockaddr* sa) {
    if(sa->sa_family == AF_INET) return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

static struct addrinfo* init_addrinfo(const char* host, const char* port, const struct addrinfo* hints) {
    int res;
    struct addrinfo* info;
    if((res = getaddrinfo(host, port, hints, &info)) != 0) return NULL;
    return info;
}

static int bind_server(scl_socket_server* server) {
    if(!server) return -1;
    struct addrinfo hints = {
        .ai_family      = AF_UNSPEC,
        .ai_socktype    = SOCK_STREAM,
        .ai_flags       = AI_PASSIVE,
    };
    struct addrinfo* info = init_addrinfo(NULL, server->port, &hints);
    inet_ntop(info->ai_family, get_in_addr(info->ai_addr), server->ip, sizeof(server->ip));
    SCL_VLOG("starting server with address: %s:%s", server->ip, server->port);
    int opt = 1;
    for(server->sai = info; server->sai != NULL; server->sai = server->sai->ai_next) {
        if((server->fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) == -1) 
	    continue;
        if(setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
            return -1;
        if(bind(server->fd, info->ai_addr, info->ai_addrlen) == -1) {
            close(server->fd);
            continue;
        }
    break;
    }
    freeaddrinfo(info);
    return 0;
}

int scl_socket_server_init(scl_socket_server* server) {
    int res = bind_server(server);
    if(res == -1) return -1;
    if(listen(server->fd, 10) == -1) return -1;
    if(kill_dead() == -1) return -1;
    SCL_LOG("server awaiting connections...");
    return 0;
}

int scl_socket_server_accept(scl_socket_server* server, void (*func)(int)) {
    socklen_t ra_len = sizeof(server->ra);
    if((server->rfd = accept(server->fd, (struct sockaddr*)&server->ra, &ra_len)) == -1)
        return -1;
    inet_ntop(server->ra.ss_family, get_in_addr((struct sockaddr*)&server->ra), server->rip, sizeof(server->rip));
    SCL_VLOG("server connected with ip: %s", server->rip);
    if(!fork()) {
        int rfd = server->rfd;
        if(func) func(rfd);
    }
    close(server->rfd);
    return 0;
}

void scl_socket_server_clean(scl_socket_server* server) {
    freeaddrinfo(server->sai);
}

static int connect_client(scl_socket_client* client) {
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
    return 0;
}

int scl_socket_client_init(scl_socket_client* client) {
    int res = connect_client(client);
    if(res == -1) return -1;
    char rip[INET6_ADDRSTRLEN];
    SCL_VLOG("client connected to address %s:%s", rip, client->port);
    return 0;
}
