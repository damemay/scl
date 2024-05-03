#include "../data/hmap.h"
#include "../math/hash.h"
#include "../math/prime.h"
#include <string.h>

scl_hmap* scl_hmap_new() {
    scl_hmap* hmap = malloc(sizeof(scl_hmap));
    if(!hmap) return NULL;
    if(scl_array_init(hmap, 53, sizeof(scl_mitem*)) == -1) {
	free(hmap);
	return NULL;
    }
    return hmap;
}

static int resize(scl_hmap* hmap, const int size) {
    if(size < 53) return -1;
    int new_prime = scl_next_prime(size);
    if(__scl_array_realloc(hmap,new_prime) == -1) return -1;
    hmap->capacity = new_prime;
    return 0;
}

int scl_hmap_add(scl_hmap* hmap, const char* key, const void* value, const size_t size) {
    const int load = hmap->size * 100 / hmap->capacity;
    if(load > 70) resize(hmap, hmap->capacity*2);
    scl_mitem* item = scl_mitem_new(key, value, size);
    if(!item) return -1;
    int index = scl_hash_get(key, hmap->capacity, 0);
    scl_mitem** data = hmap->data;
    scl_mitem* cur_item = data[index];
    for(size_t i=1; cur_item != NULL; i++) {
	if(strcmp(cur_item->key, key) == 0) {
	    scl_mitem_free(cur_item);
	    data[index] = item;
	    return 0;
	}
	index = scl_hash_get(key, hmap->capacity, i);
	cur_item = data[index];
    }
    data[index] = item;
    hmap->size++;
    return 0;
}

void* scl_hmap_get(scl_hmap* hmap, const char* key) {
    int index = scl_hash_get(key, hmap->capacity, 0);
    scl_mitem** data = hmap->data;
    scl_mitem* item = data[index];
    for(size_t i=1; item != NULL; i++) {
	if(strcmp(item->key, key) == 0)
	    return item->value;
	index = scl_hash_get(key, hmap->capacity, i);
	item = data[index];
    }
    return NULL;
}

int scl_hmap_del(scl_hmap* hmap, const char* key) {
    int index = scl_hash_get(key, hmap->capacity, 0);
    scl_mitem** data = hmap->data;
    scl_mitem* item = data[index];
    for(size_t i=1; item != NULL; i++) {
	if(strcmp(item->key, key) == 0) {
	    scl_mitem_free(item);
	    data[index] = NULL;
	    hmap->size--;
	    const int load = hmap->size * 100 / hmap->capacity;
	    if(load < 10) resize(hmap, hmap->capacity/2);
	    return 0;
	}
	index = scl_hash_get(key, hmap->capacity, i);
	item = data[index];
    }
    return -1;
}

void scl_hmap_iterate(scl_hmap* hmap, void (*func)(scl_mitem*, void*), void* user_data) {
    scl_mitem** data = hmap->data;
    for(size_t i=1; i<hmap->capacity; i++) {
	scl_mitem* item = data[i];
	if(!item) continue;
	if(func) func(item, user_data);
    }
}

void scl_hmap_free(scl_hmap* hmap) {
    scl_mitem** data = hmap->data;
    for(size_t i=0; i<hmap->capacity; i++) {
	scl_mitem* item = data[i];
	if(item) scl_mitem_free(item);
    }
    scl_array_free(hmap);
    free(hmap);
}
