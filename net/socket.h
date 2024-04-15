#ifndef __SCL_SOCKETS_H__
#define __SCL_SOCKETS_H__
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

int scl_send(int fd, const void* buf, const uint32_t size);
int scl_recv(int fd, void* buf, const uint32_t size);

typedef struct scl_socket_server {
    const char* port;
    struct addrinfo* sai;
    int fd;
    char ip[INET6_ADDRSTRLEN];

    struct sockaddr_storage ra;
    int rfd;
    char rip[INET6_ADDRSTRLEN];
} scl_socket_server;

int scl_socket_server_init(scl_socket_server* server);
int scl_socket_server_accept(scl_socket_server* server, void (*func)(int));
void scl_socket_server_free(scl_socket_server* server);

typedef struct scl_socket_client {
    const char* host;
    const char* port;
    struct addrinfo* ai;
    int fd;
} scl_socket_client;

int scl_socket_client_init(scl_socket_client* client);
#endif
