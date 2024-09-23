#include "../scl.h"
#include <stdio.h>
#include <assert.h>

int main() {
    scl_dic* dict = scl_dic_new();
    int res = scl_dic_add(dict, "ABC", "String insertion");
    assert(res==0);
    res = scl_dic_add(dict, "DEF", "Test string 2");
    assert(res==0);
    res = scl_dic_add(dict, "GHIJKLMNO", "Yet another string inside dictionary");
    assert(res==0);

    void* x = scl_dic_get(dict, "ABC");
    assert(x);
    printf("%s\n", (char*)x);
    x = scl_dic_get(dict, "DEF");
    assert(x);
    printf("%s\n", (char*)x);
    x = scl_dic_get(dict, "GHIJKLMNO");
    assert(x);
    printf("%s\n", (char*)x);

    scl_dic_free(dict);
}
