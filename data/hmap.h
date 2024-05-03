#ifndef __SCL_HMAP_H__
#define __SCL_HMAP_H__

#include "map.h"

typedef scl_array scl_hmap;

scl_hmap* scl_hmap_new();
int scl_hmap_add(scl_hmap* hmap, const char* key, const void* value, const size_t size);
int scl_hmap_del(scl_hmap* hmap, const char* key);
void* scl_hmap_get(scl_hmap* hmap, const char* key);
void scl_hmap_iterate(scl_hmap* hmap, void (*func)(scl_mitem*, void*), void* user_data);
void scl_hmap_free(scl_hmap* hmap);

#endif
