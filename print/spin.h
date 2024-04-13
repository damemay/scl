#ifndef __SCL_SPIN_H__
#define __SCL_SPIN_H__
#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define SCL_SPIN_ELEMENTS 4

typedef struct scl_spin {
    char elements[SCL_SPIN_ELEMENTS];
    int delay;
    bool busy;
    size_t cur;
    pthread_t thread;
    const char* msg;
} scl_spin;

int scl_spin_start(scl_spin* spin, const char* msg);
void scl_spin_cancel(scl_spin* spin);
int scl_spin_restart(scl_spin* spin, const char* msg);
#endif
