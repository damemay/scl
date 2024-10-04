#ifndef SMOL_C_LIB_H
#define SMOL_C_LIB_H
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sarr {
    void** data;
    size_t cap;
    size_t len;
} sarr;

sarr* sarr_new();
int sarr_add(sarr* array, const void* element);
void* sarr_get(sarr* array, size_t index);
void sarr_free(sarr* array);

struct sdic_i {
    char* key;
    void* value;
};
typedef sarr sdic;
sdic* sdic_new();
int sdic_add(sdic* dict, const char* key, const void* value);
void* sdic_get(sdic* dict, const char* key);
void sdic_free(sdic* dict);

struct sfn {
    void (*fn)(void* arg);
    void* arg;
};
typedef sarr sfnq;
sfnq* sfnq_new();
int sfnq_add(sfnq* queue, void(*fn)(void*arg), void* arg);
void sfnq_fend(sfnq* queue);
void sfnq_fstart(sfnq* queue);
void sfnq_free(sfnq* queue);

char* sread(const char* filepath, int nul_terminate, size_t* size);
char** sreadlns(const char* filepath, size_t* len);

#ifdef __cplusplus
}
#endif

#endif
