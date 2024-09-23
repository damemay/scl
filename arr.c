#include <stdint.h>
#include <string.h>
#include "scl.h"

scl_arr* scl_arr_new(const size_t type_size) {
    scl_arr* array = malloc(sizeof(scl_arr));
    if(!array) return NULL;
    array->len = 0;
    array->cap = 16;
    array->t_size = type_size;
    array->data = calloc(16, sizeof(type_size));
    if(!array->data) {
	free(array);
	return NULL;
    }
    return array;
}

int scl_arr_add(scl_arr* array, const void* element) {
    if(array->len >= array->cap) {
	size_t n_cap = array->cap * 2;
	void** data = realloc(array->data, n_cap * sizeof(array->t_size));
	if(!data) return -1;
	array->data = data;
	array->cap = n_cap;
    }
    array->data[array->len++] = (void*)element;
    return 0;
}

void* scl_arr_get(scl_arr* array, size_t index) {
    if(index >= array->len || index < 0) return NULL;
    return array->data[index];
}

void scl_arr_free(scl_arr* array) {
    free(array->data);
    free(array);
}
