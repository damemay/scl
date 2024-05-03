#ifndef __SCL_ARRAY_H__
#define __SCL_ARRAY_H__
#include <stdint.h>
#include <stdlib.h>

typedef struct scl_array {
    uint64_t capacity;
    uint64_t size;
    uint64_t step;
    size_t type_size;
    void* data;
} scl_array;

int scl_array_init(scl_array* array, const uint64_t capacity, const size_t type_size);
void scl_array_free(scl_array* array);
int __scl_array_realloc(scl_array* array, const uint64_t new_capacity);
int __scl_array_index_exists(scl_array* array, const uint64_t index);
int __scl_array_can_add(scl_array* array);
void __scl_array_del_realloc(scl_array* array);

#define SCL_ARRAY_ADD(ARRAY, ELEMENT, TYPE) ({ \
    int __SCL_RETURN_CODE__ = -1; \
    if(__scl_array_can_add(ARRAY)) { \
	TYPE* __SCL_DATA__ = (TYPE*)ARRAY->data; \
	__SCL_DATA__[ARRAY->size++] = ELEMENT; \
	ARRAY->data = __SCL_DATA__; \
	__SCL_RETURN_CODE__ = 0; \
    } \
    __SCL_RETURN_CODE__; \
})

#define SCL_ARRAY_DEL(ARRAY, INDEX, TYPE) ({ \
    int __SCL_RETURN_CODE__ = -1; \
    if(__scl_array_index_exists) { \
	TYPE* __SCL_DATA__ = (TYPE*)ARRAY->data; \
	for(size_t __SCL_ITERATOR__=INDEX+1; __SCL_ITERATOR__<ARRAY->size; __SCL_ITERATOR__++) \
	    __SCL_DATA__[__SCL_ITERATOR__-1] = __SCL_DATA__[__SCL_ITERATOR__]; \
	ARRAY->size -= 1; \
	__scl_array_del_realloc(ARRAY); \
	__SCL_RETURN_CODE__ = 0; \
    } \
    __SCL_RETURN_CODE__; \
})

#define SCL_ARRAY_GET(ARRAY, INDEX, TYPE) ({ \
    TYPE __SCL_RETURN_DATA__; \
    if(__scl_array_index_exists(ARRAY, INDEX)) { \
	TYPE* __SCL_DATA__ = (TYPE*)ARRAY->data; \
	__SCL_RETURN_DATA__ = __SCL_DATA__[INDEX]; \
    } \
    __SCL_RETURN_DATA__; \
})

#endif
