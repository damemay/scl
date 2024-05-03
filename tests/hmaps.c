#include "../data/hmap.h"
#include "../utils/helpers.h"
#include <assert.h>
#include <string.h>

void print_hmap(scl_mitem* item, void* data) {
    printf("%s:%s\n", item->key, (char*)item->value);
}

int main() {
    scl_hmap* hmap = scl_hmap_new();
    int res = scl_hmap_add(hmap, "first", "tes1", 5);
    assert(res == 0);
    res = scl_hmap_add(hmap, "second", "tes2", 5);
    assert(res == 0);
    res = scl_hmap_add(hmap, "third", "tes3", 5);
    assert(res == 0);
    char* value = scl_hmap_get(hmap, "first");
    assert(strcmp(value, "tes1") == 0);
    SCL_VLOG("first: %s", value);
    res = scl_hmap_add(hmap, "first", "tes10", 6);
    assert(res == 0);
    value = scl_hmap_get(hmap, "first");
    assert(strcmp(value, "tes10") == 0);
    SCL_VLOG("first: %s", value);
    value = scl_hmap_get(hmap, "third");
    assert(strcmp(value, "tes3") == 0);
    SCL_VLOG("third: %s", value);
    res = scl_hmap_del(hmap, "third");
    assert(res == 0);
    value = scl_hmap_get(hmap, "third");
    assert(value == NULL);
    SCL_LOG("third: not found");
    scl_hmap_iterate(hmap, print_hmap, NULL);
}
