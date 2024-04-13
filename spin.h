#ifndef __SECL_SPIN_H__
#define __SECL_SPIN_H__
#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define SECL_SPIN_ELEMENTS 4

typedef struct secl_spin {
    char elements[SECL_SPIN_ELEMENTS];
    int delay;
    bool busy;
    size_t cur;
    pthread_t thread;
    const char* msg;
} secl_spin;

int secl_spin_start(secl_spin* spin, const char* msg);
void secl_spin_cancel(secl_spin* spin);
int secl_spin_restart(secl_spin* spin, const char* msg);
#endif
