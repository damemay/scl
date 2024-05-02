#ifndef __SCL_MAP_H__
#define __SCL_MAP_H__
#include "array.h"

typedef struct scl_mitem {
    char* key;
    void* value;
} scl_mitem;

typedef scl_array scl_map;

scl_map* scl_map_create(const uint64_t initial_capacity);
int scl_map_add(scl_map* map, const char* key, const void* value, const size_t size);
int scl_map_del(scl_map* map, const char* key);
void* scl_map_get(scl_map* map, const char* key);
scl_mitem* scl_map_get_by_index(scl_map* map, const uint64_t index);
void scl_map_free(scl_map* map);

#endif
