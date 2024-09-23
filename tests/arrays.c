#include "../scl.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char** argv) {
    scl_arr* list = scl_arr_new(sizeof(char*));
    assert(list);
    int res = scl_arr_add(list, "Abcdef");
    assert(res == 0);
    void* x = scl_arr_get(list, 0);
    printf("%d %s\n", 0, (char*)x);

    scl_arr_free(list);
}
