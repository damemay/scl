#ifndef __SECL_SOCKETS_H__
#define __SECL_SOCKETS_H__
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

int secl_send(int fd, const void* buf, const uint32_t size);
int secl_recv(int fd, void* buf, const uint32_t size);

typedef struct secl_server {
    const char* port;
    struct addrinfo* sai;
    int fd;
    char ip[INET6_ADDRSTRLEN];

    struct sockaddr_storage ra;
    int rfd;
    char rip[INET6_ADDRSTRLEN];
} secl_server;

int secl_server_init(secl_server* server);
int secl_server_accept(secl_server* server, void (*func)(int));
void secl_server_clean(secl_server* server);

typedef struct secl_client {
    const char* host;
    const char* port;
    struct addrinfo* ai;
    int fd;
} secl_client;

int secl_client_init(secl_client* client);
void zkt_client_clean(secl_client* client);
#endif
