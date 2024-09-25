#include "../scl.h"
#include <stdio.h>
#include <assert.h>

int main() {
    sdic* dict = sdic_new();
    int res = sdic_add(dict, "ABC", "String insertion");
    assert(res==0);
    res = sdic_add(dict, "DEF", "Test string 2");
    assert(res==0);
    res = sdic_add(dict, "GHIJKLMNO", "Yet another string inside dictionary");
    assert(res==0);

    void* x = sdic_get(dict, "ABC");
    assert(x);
    printf("%s\n", (char*)x);
    x = sdic_get(dict, "DEF");
    assert(x);
    printf("%s\n", (char*)x);
    x = sdic_get(dict, "GHIJKLMNO");
    assert(x);
    printf("%s\n", (char*)x);

    sdic_free(dict);
}
