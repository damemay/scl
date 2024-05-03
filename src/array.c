#include "../data/array.h"
#include <stdint.h>

int scl_array_init(scl_array* array, const uint64_t capacity, const size_t type_size) {
    array->capacity = capacity;
    array->step = capacity;
    array->size = 0;
    array->type_size = type_size;
    array->data = calloc(array->capacity, array->type_size);
    if(!array->data) return -1;
    return 0;
}

int __scl_array_realloc(scl_array* array, const uint64_t new_capacity) {
    void* data = realloc(array->data, new_capacity*array->type_size);
    if(!data) return -1;
    array->data = data;
    return 0;
}

int __scl_array_can_add(scl_array* array) {
    if(array->size+1 > array->capacity) {
	if(__scl_array_realloc(array, array->capacity+array->step) == -1) return 0;
	array->capacity += array->step;
    }
    return 1;
}

int __scl_array_index_exists(scl_array* array, const uint64_t index) {
    if(index >= array->size || index < 0) return 0;
    return 1;
}

void __scl_array_del_realloc(scl_array* array) {
    if(array->size < array->capacity-array->step)
	if(__scl_array_realloc(array, array->capacity-array->step) == 0)
	    array->capacity -= array->step;
}

void scl_array_free(scl_array* array) {
    free(array->data);
}
