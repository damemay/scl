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

#define scl_array_init(ARRAY, TYPE, CAPACITY) ({ \
    ARRAY->capacity = CAPACITY; \
    ARRAY->step = CAPACITY; \
    ARRAY->size = 0; \
    ARRAY->data = calloc(SIZE, sizeof(TYPE)); \
    if(!ARRAY->data) -1; \
    0; \
})

#define scl_array_realloc(ARRAY, TYPE, CAPACITY) ({ \
    void* new_data = realloc(ARRAY->data, CAPACITY*sizeof(TYPE)); \
    if(!new_data) -1; \
    ARRAY->data = new_data; \
    0; \
})

#define scl_array_add(ARRAY, TYPE, ELEMENT) ({ \
    if(ARRAY->size+1 > ARRAY->capacity) \
	if(scl_array_realloc(ARRAY, TYPE, ARRAY->capacity+ARRAY->step) == -1) -1; \
    TYPE* data = (TYPE*)ARRAY->data; \
    data[ARRAY->size++] = ELEMENT; \
    ARRAY->data = data; \
    0; \
})

#define scl_array_get(ARRAY, TYPE, INDEX) ({ \
    if(INDEX >= ARRAY->size || INDEX < 0) -1; \
    TYPE* data = (TYPE*)ARRAY->data; \
    data[INDEX]; \
})

#define scl_array_del(ARRAY, TYPE, INDEX) ({ \
    if(INDEX >= ARRAY->size || INDEX < 0) -1; \
    TYPE* data = (TYPE*)ARRAY->data; \
    for(size_t i=INDEX+1; i<ARRAY->size; i++) \
	data[i-1] = data[i]; \
    ARRAY->size -= 1; \
    if(ARRAY->size < ARRAY->capacity-ARRAY->step) \
	if(scl_array_realloc(ARRAY, TYPE, ARRAY->capacity-ARRAY->step) == -1) -1; \
    0; \
})

static inline void scl_array_free(scl_array* array) {
    free(array->data);
}
#endif
