#ifndef __SCL_ARRAY_H__
#define __SCL_ARRAY_H__
#include <stdint.h>
#include <stdlib.h>

typedef struct scl_array {
    uint32_t capacity;
    uint32_t size;
    uint32_t step;
    void* data;
} scl_array;

#define SCL_ARRAY_INIT(ARRAY, TYPE, CAPACITY) ({ \
    int __SCL_RETURN_CODE__ = 0; \
    ARRAY->capacity = CAPACITY; \
    ARRAY->step = CAPACITY; \
    ARRAY->size = 0; \
    ARRAY->data = calloc(ARRAY->capacity, sizeof(TYPE)); \
    if(!ARRAY->data) __SCL_RETURN_CODE__ = -1; \
    __SCL_RETURN_CODE__; \
})

#define SCL_ARRAY_REALLOC(ARRAY, TYPE, CAPACITY) ({ \
    int __SCL_RETURN_CODE__ = 0; \
    void* __SCL_DATA__ = realloc(ARRAY->data, CAPACITY*sizeof(TYPE)); \
    if(!__SCL_DATA__) __SCL_RETURN_CODE__ = -1; \
    else ARRAY->data = __SCL_DATA__; \
    __SCL_RETURN_CODE__; \
})

#define SCL_ARRAY_ADD(ARRAY, TYPE, ELEMENT) ({ \
    int __SCL_RETURN_CODE__ = 0; \
    if(ARRAY->size+1 > ARRAY->capacity) { \
	if(SCL_ARRAY_REALLOC(ARRAY, TYPE, ARRAY->capacity+ARRAY->step) == -1) __SCL_RETURN_CODE__ = -1; \
	else ARRAY->capacity += ARRAY->step; \
    } \
    if(__SCL_RETURN_CODE__ == 0) { \
	TYPE* __SCL_DATA__ = (TYPE*)ARRAY->data; \
	__SCL_DATA__[ARRAY->size++] = ELEMENT; \
	ARRAY->data = __SCL_DATA__; \
    } \
    __SCL_RETURN_CODE__; \
})

#define SCL_ARRAY_GET(ARRAY, TYPE, INDEX, ELEMENT) ({ \
    int __SCL_RETURN_CODE__ = 0; \
    if(INDEX >= ARRAY->size || INDEX < 0) __SCL_RETURN_CODE__ = -1; \
    else { \
	TYPE* __SCL_DATA__ = (TYPE*)ARRAY->data; \
	ELEMENT = __SCL_DATA__[INDEX]; \
    } \
    __SCL_RETURN_CODE__; \
})

#define SCL_ARRAY_DEL(ARRAY, TYPE, INDEX) ({ \
    int __SCL_RETURN_CODE__ = 0; \
    if(INDEX >= ARRAY->size || INDEX < 0) __SCL_RETURN_CODE__ = -1; \
    else { \
	TYPE* __SCL_DATA__ = (TYPE*)ARRAY->data; \
	for(size_t __SCL_ITERATOR__=INDEX+1; __SCL_ITERATOR__<ARRAY->size; __SCL_ITERATOR__++) \
	    __SCL_DATA__[__SCL_ITERATOR__-1] = __SCL_DATA__[__SCL_ITERATOR__]; \
	ARRAY->size -= 1; \
    } \
    if(ARRAY->size < ARRAY->capacity-ARRAY->step) { \
	if(SCL_ARRAY_REALLOC(ARRAY, TYPE, ARRAY->capacity-ARRAY->step) == 0) \
	    ARRAY->capacity -= ARRAY->step; \
    } \
    __SCL_RETURN_CODE__; \
})

static inline void scl_array_free(scl_array* array) {
    free(array->data);
}
#endif
