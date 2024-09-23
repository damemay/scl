#ifndef SCL_H
#define SCL_H
#include <stdint.h>
#include <stdlib.h>

typedef struct scl_arr {
    void** data;
    size_t cap;
    size_t len;
    size_t t_size;
} scl_arr;

scl_arr* scl_arr_new(const size_t type_size);
int scl_arr_add(scl_arr* array, const void* element);
void* scl_arr_get(scl_arr* array, size_t index);
void scl_arr_free(scl_arr* array);

struct scl_di {
    char* key;
    void* value;
};
typedef scl_arr scl_dic;
scl_dic* scl_dic_new();
int scl_dic_add(scl_dic* dict, const char* key, const void* value);
void* scl_dic_get(scl_dic* dict, const char* key);
void scl_dic_free(scl_dic* dict);

char* scl_rd(const char* filepath, int nul_terminate, size_t* size);
char** scl_rdl(const char* filepath, size_t* len);

#endif
