#include "../scl.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char** argv) {
    sarr* list = sarr_new();
    assert(list);
    int res = sarr_add(list, "Abcdef");
    assert(res == 0);
    void* x = sarr_get(list, 0);
    printf("%d %s\n", 0, (char*)x);

    sarr_free(list);
}
