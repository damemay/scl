#include "../data/map.h"
#include <assert.h>
#include "../utils/helpers.h"

void print_map(scl_map* map) {
    for(size_t i=0; i<map->size; i++) {
	scl_mitem* item;
	SCL_ARRAY_GET(map, scl_mitem*, i, item);
	printf("%zu '%s':'%s'\n", i, item->key, (char*)item->value);
    }
}

int main(int argc, char** argv) {
    scl_map* map = scl_map_init(10);
    int res = scl_map_add(map, "first", "test", 5);
    assert(res == 0);
    res = scl_map_add(map, "first", "test", 5);
    assert(res == -1);
    res = scl_map_add(map, "second", "test2", 5);
    assert(res == 0);
    res = scl_map_add(map, "third", "test3", 5);
    assert(res == 0);
    SCL_LOG("map:");
    print_map(map);
    SCL_VLOG("size %d; capacity %d", map->size, map->capacity);
    SCL_LOG("get second:");
    char* value = scl_map_get(map, "second");
    SCL_VLOG("%s", value);
    SCL_LOG("deleting second:");
    res = scl_map_del(map, "second");
    assert(res == 0);
    SCL_VLOG("size %d; capacity %d", map->size, map->capacity);
    SCL_LOG("get third:");
    value = scl_map_get(map, "third");
    SCL_VLOG("%s", value);
    SCL_LOG("get second:");
    value = scl_map_get(map, "second");
    SCL_VLOG("%s", value);
    SCL_LOG("map:");
    print_map(map);

    scl_map_free(map);
    return 0;
}
