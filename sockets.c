#include "sockets.h"
#include <netdb.h>
#include <stdint.h>
#include <stdlib.h>

int secl_send(int fd, const void* buf, const uint32_t size) {
    size_t total = 0, left = size, ret = 0;
    while(total < size) {
        ret = send(fd, buf+total, left, 0);
        if(ret == -1) return -1;
        total += ret;
        left -= ret;
    }
    return total;
}

int secl_recv(int fd, void* buf, const uint32_t size) {
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
    return 1;
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

static int bind_server(secl_server* server) {
    if(!server) return -1;
    struct addrinfo hints = {
        .ai_family      = AF_UNSPEC,
        .ai_socktype    = SOCK_STREAM,
        .ai_flags       = AI_PASSIVE,
    };
    struct addrinfo* info = init_addrinfo(NULL, server->port, &hints);
    inet_ntop(info->ai_family, get_in_addr(info->ai_addr), server->ip, sizeof(server->ip));
    secl_vlog("starting server with address: %s:%s", server->ip, server->port);
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
    return 1;
}

int secl_server_init(secl_server* server) {
    int res = bind_server(server);
    if(res == -1) return -1;
    if(listen(server->fd, 10) == -1) return -1;
    if(kill_dead() == -1) return -1;
    secl_log("server awaiting connections...");
    return 1;
}

int secl_server_accept(secl_server* server, void (*func)(int)) {
    socklen_t ra_len = sizeof(server->ra);
    if((server->rfd = accept(server->fd, (struct sockaddr*)&server->ra, &ra_len)) == -1)
        return -1;
    inet_ntop(server->ra.ss_family, get_in_addr((struct sockaddr*)&server->ra), server->rip, sizeof(server->rip));
    secl_vlog("server connected with ip: %s", server->rip);
    if(!fork()) {
        int rfd = server->rfd;
        if(func) func(rfd);
    }
    close(server->rfd);
    return 1;
}

void secl_server_clean(secl_server* server) {
    freeaddrinfo(server->sai);
}

static int connect_client(secl_client* client) {
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

int secl_client_init(secl_client* client) {
    int res = connect_client(client);
    if(res == -1) return -1;
    char rip[INET6_ADDRSTRLEN];
    secl_vlog("client connected to address %s:%s", rip, client->port);
    return 1;
}

void secl_client_clean(secl_client* client) {
    freeaddrinfo(client->ai);
}
