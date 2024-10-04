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

    sarr* list2 = sarr_new();
    int a = 1, b = 0, c = 2;
    assert(list2);
    res = sarr_add(list2, &a);
    assert(res == 0);
    res = sarr_add(list2, &b);
    assert(res == 0);
    res = sarr_add(list2, &c);
    assert(res == 0);
    for(size_t i = 0; i < list2->len; i++) {
	x = sarr_get(list2, i);
	printf("%d\n", *(int*)x);
    }

    sarr_free(list);
}
