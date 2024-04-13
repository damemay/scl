#ifndef __TINYSOCKETS_H__
#define __TINYSOCKETS_H__
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

int send_all(int fd, const void* buf, const uint32_t size);
int recv_all(int fd, void* buf, const uint32_t size);

typedef struct tinyclient {
    const char* host;
    const char* port;
    struct addrinfo* ai;
    int fd;
} tinyclient;

int tinyclient_conn(tinyclient* client);
void tinyclient_free(tinyclient* client);

#endif
