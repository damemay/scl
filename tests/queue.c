#include "../scl.h"
#include <assert.h>
#include <stdio.h>

void p1(void* arg) {
    puts("1");
}
void p2(void* arg) {
    puts("2");
}
void p3(void* arg) {
    puts("3");
}
int main(int argc, char** argv) {
    sfnq* queue = sfnq_new();
    assert(queue);
    int res = sfnq_add(queue, p1, NULL);
    assert(res == 0);
    res = sfnq_add(queue, p2, NULL);
    assert(res == 0);
    res = sfnq_add(queue, p3, NULL);
    assert(res == 0);
    sfnq_fstart(queue);
    puts("");
    sfnq_fend(queue);
    sfnq_free(queue);
}

