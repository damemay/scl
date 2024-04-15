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
    int ret = 0; \
    ARRAY->capacity = CAPACITY; \
    ARRAY->step = CAPACITY; \
    ARRAY->size = 0; \
    ARRAY->data = calloc(ARRAY->capacity, sizeof(TYPE)); \
    if(!ARRAY->data) ret = -1; \
    ret; \
})

#define SCL_ARRAY_REALLOC(ARRAY, TYPE, CAPACITY) ({ \
    int ret = 0; \
    void* new_data = realloc(ARRAY->data, CAPACITY*sizeof(TYPE)); \
    if(!new_data) ret = -1; \
    else ARRAY->data = new_data; \
    ret; \
})

#define SCL_ARRAY_ADD(ARRAY, TYPE, ELEMENT) ({ \
    int ret = 0; \
    if(ARRAY->size+1 > ARRAY->capacity) { \
	if(SCL_ARRAY_REALLOC(ARRAY, TYPE, ARRAY->capacity+ARRAY->step) == -1) ret = -1; \
	else ARRAY->capacity += ARRAY->step; \
    } \
    if(ret == 0) { \
	TYPE* data = (TYPE*)ARRAY->data; \
	data[ARRAY->size++] = ELEMENT; \
	ARRAY->data = data; \
    } \
    ret; \
})

#define SCL_ARRAY_GET(ARRAY, TYPE, INDEX, ELEMENT) ({ \
    int ret = 0; \
    if(INDEX >= ARRAY->size || INDEX < 0) ret = -1; \
    else { \
	TYPE* data = (TYPE*)ARRAY->data; \
	ELEMENT = data[INDEX]; \
    } \
    ret; \
})

#define SCL_ARRAY_DEL(ARRAY, TYPE, INDEX) ({ \
    int ret = 0; \
    if(INDEX >= ARRAY->size || INDEX < 0) ret = -1; \
    else { \
	TYPE* data = (TYPE*)ARRAY->data; \
	for(size_t i=INDEX+1; i<ARRAY->size; i++) \
	    data[i-1] = data[i]; \
	ARRAY->size -= 1; \
    } \
    if(ARRAY->size < ARRAY->capacity-ARRAY->step) { \
	if(SCL_ARRAY_REALLOC(ARRAY, TYPE, ARRAY->capacity-ARRAY->step) == 0) \
	    ARRAY->capacity -= ARRAY->step; \
    } \
    ret; \
})

static inline void scl_array_free(scl_array* array) {
    free(array->data);
}
#endif
