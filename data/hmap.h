#ifndef __SCL_HMAP_H__
#define __SCL_HMAP_H__

#include "map.h"

typedef scl_array scl_hmap;

scl_hmap* scl_hmap_new();
int scl_hmap_add(scl_hmap* hmap, const char* key, const void* value, const size_t size);
int scl_hmap_del(scl_hmap* hmap, const char* key);
void* scl_hmap_get(scl_hmap* hmap, const char* key);
scl_mitem* scl_hmap_get_by_index(scl_hmap* hmap, const uint64_t index);
void scl_hmap_free(scl_hmap* hmap);

#endif
