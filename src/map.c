#include "../data/map.h"
#include "../utils/helpers.h"
#include <string.h>

static void scl_mitem_free(scl_mitem* item) {
    free(item->key);
    free(item->value);
    free(item);
}

scl_map* scl_map_create(uint32_t capacity) {
    scl_map* map = malloc(sizeof(scl_map));
    if(!map) return NULL;
    if(scl_array_init(map, capacity, sizeof(scl_mitem*)) == -1) {
	free(map);
	return NULL;
    }
    return map;
}

int scl_map_add(scl_map* map, const char* key, const void* value, size_t size) {
    if(scl_map_get(map, key)) return -1;
    scl_mitem* item = malloc(sizeof(scl_mitem));
    if(!item) return -1;
    item->key = strdup(key);
    if(!item->key) {
	free(item);
	return -1;
    }
    item->value = malloc(size);
    if(!item->value) {
	free(item->key);
	free(item);
	return -1;
    }
    memcpy(item->value, value, size);
    if(SCL_ARRAY_ADD(map, item, scl_mitem*) == -1) return -1;
    return 0;
}

int scl_map_del(scl_map* map, const char* key) {
    for(size_t i=0; i<map->size; i++) {
	scl_mitem* item = SCL_ARRAY_GET(map, i, scl_mitem*);
	if(!item) return -1;
	if(strcmp(item->key, key) == 0) {
	    SCL_VLOG("removing %s:%s", item->key, (char*)item->value);
	    scl_mitem_free(item);
	    SCL_ARRAY_DEL(map, i, scl_mitem*);
	    return 0;
	}
    }
    return -1;
}

void* scl_map_get(scl_map* map, const char* key) {
    for(size_t i=0; i<map->size; i++) {
	scl_mitem* item = SCL_ARRAY_GET(map, i, scl_mitem*);
	if(!item) return NULL;
	if(strcmp(item->key, key) == 0)
	    return item->value;
    }
    return NULL;
}

scl_mitem* scl_map_get_by_index(scl_map* map, uint32_t index) {
    scl_mitem* item = SCL_ARRAY_GET(map, index, scl_mitem*);
    if(!item) return NULL;
    return item;
}

void scl_map_free(scl_map* map) {
    for(size_t i=0; i<map->size; i++) {
	scl_mitem* item = SCL_ARRAY_GET(map, i, scl_mitem*);
       	if(item) scl_mitem_free(item);
    }
    scl_array_free(map);
    free(map);
}
