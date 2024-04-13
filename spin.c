#include "cspin.h"

static inline void* work(void* arg) {
    secl_spin* t = (secl_spin*)arg;
    printf("%s ", t->msg);
    while(t->busy) {
        putchar(t->elements[(t->cur++)%SECL_SPIN_ELEMENTS]);
        fflush(stdout);
        usleep(t->delay);
        putchar('\b');
        fflush(stdout);
    }
    pthread_exit(0);
}

int secl_spin_start(secl_spin* spin, const char* msg) {
    spin->elements[0] = '-', spin->elements[1] = '/',
    spin->elements[2] = '|', spin->elements[3] = '\\';
    spin->delay = 100000;
    spin->busy = true;
    spin->cur = 0;
    spin->msg = NULL;
    spin->msg = msg;
    if(pthread_create(&spin->thread, NULL, work, spin)) return -1;
    return 0;
}

void secl_spin_cancel(secl_spin* spin) {
    pthread_cancel(spin->thread);
    pthread_join(spin->thread, NULL);
    fprintf(stdout, "\b \r");
}

int cspin_restart(secl_spin* spin, const char* msg) {
    if(msg) spin->msg = msg;
    if(pthread_create(&spin->thread, NULL, work, spin)) return -1;
    return 0;
}
