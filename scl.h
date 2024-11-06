//
// smol c library - public domain utility library
// No warranty implied. Use at your own risk.
//
// Define SCL_IMPLEMENTATION in one source file before including header:
//  #define SCL_IMPLEMENTATION
//  #include "scl.h"
//
// If you want to use data structures on non-pointer values,
// do this in the same source file:
//  SCL_SARR_IMPLEMENT(int); // for sarr_int
//  SCL_SLIST_IMPLEMENT(int); // for slist_int
//  SCL_SHTABLE_IMPLEMENT(int); // for shtable_int
//
// The source file should look then like this:
//  #define SCL_IMPLEMENTATION
//  #include "scl.h"
//  SCL_SARR_IMPLEMENT(int);
//  SCL_SLIST_IMPLEMENT(int);
//  SCL_SHTABLE_IMPLEMENT(int);
//
// If you implement non-pointer data structures,
// you also need to define them in one of your header files to use them:
//  SCL_SARR_DEFINE(int); // for sarr_int
//  SCL_SLIST_DEFINE(int); // for slist_int
//  SCL_SHTABLE_DEFINE(int); // for shtable_int
//
// NOTE: All data structures don't take ownership of values inserted.
// If you allocate a value and insert it into data structure as a pointer,
// you're responsible for freeing it.
//
// sread/sreadlns: if you need to read file bigger than 2GB on a 32-bit device
// and you're including <stdio.h> before scl.h,
// compile your project/scl source file with -D_FILES_OFFSET_BITS=64.
//

#ifndef SMOL_C_LIBRARY
#define SMOL_C_LIBRARY
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct sarr {
  void** data;
  size_t len;
} sarr;

sarr* sarr_new(const size_t len);
void sarr_del(sarr* array);

typedef struct slist {
  void** data;
  size_t cap;
  size_t len;
} slist;

slist* slist_new(const size_t init_cap);
int slist_add(slist* list, void* element);
void slist_del(slist* list);

typedef struct shtable_item {
  char* key;
  void* value;
} shtable_item;
typedef struct shtable {
  shtable_item * data0;
  shtable_item * data1;
  size_t len;
} shtable;

shtable* shtable_new(const size_t init_len);
int shtable_add(shtable* htable, const char* key, const void* value);
int shtable_get(shtable* htable, const char* key, void** value);
void shtable_del(shtable* htable);

char* sread(const char* filepath, int nul_terminate, size_t* size);
char** sreadlns(const char* filepath, size_t* len);

void* smalloc(size_t size, const char* file, int line, const char* fn);
void* scalloc(size_t n, size_t size, const char* file, int line, const char* fn);
void sfree(void* e, const char* file, int line, const char* fn);

#define SCL_SARR_DEFINE(type) \
typedef struct sarr_ ## type { \
  type* data; \
  size_t len; \
} sarr_ ## type; \
sarr_ ## type * sarr_ ## type ## _new(const size_t len); \
void sarr_ ## type ## _del(sarr_ ## type * array)

#define SCL_SARR_IMPLEMENT(type) \
sarr_ ## type * sarr_ ## type ## _new(const size_t len) { \
  sarr_ ## type * array = (sarr_ ## type *)malloc(sizeof(sarr_ ## type )); \
  if(!array) return NULL; \
  array->len = len; \
  array->data = (type*)calloc(len, sizeof(type)); \
  if(!array->data) { \
    free(array); \
    return NULL; \
  } \
  return array; \
} \
void sarr_ ## type ## _del(sarr_ ## type * array) { \
  free(array->data); \
  free(array); \
}

#define SCL_SLIST_DEFINE(type) \
typedef struct slist_ ## type { \
  type* data; \
  size_t cap; \
  size_t len; \
} slist_ ## type; \
slist_ ## type * slist_ ## type ## _new(const size_t init_cap); \
int slist_ ## type ## _add(slist_ ## type * list, const type element); \
void slist_ ## type ## _del(slist_ ## type * list)

#define SCL_SLIST_IMPLEMENT(type) \
slist_ ## type * slist_ ## type ## _new(const size_t init_cap) { \
  slist_ ## type * list = (slist_ ## type *)malloc(sizeof(slist_ ## type )); \
  if(!list) return NULL; \
  list->cap = init_cap; \
  list->len = 0; \
  list->data = (type*)calloc(init_cap, sizeof(type)); \
  if(!list->data) { \
    free(list); \
    return NULL; \
  } \
  return list; \
} \
int slist_ ## type ## _add(slist_ ## type * list, const type element) { \
  if(list->len >= list->cap) { \
    size_t n_cap = list->cap * 2; \
    type* data = (type*)realloc(list->data, n_cap * sizeof(type)); \
    if(!data) return 0; \
    list->data = data; \
    list->cap = n_cap; \
  } \
  list->data[list->len++] = element; \
  return 1; \
} \
void slist_ ## type ## _del(slist_ ## type * list) { \
  free(list->data); \
  free(list); \
}

#define SCL_SHTABLE_DEFINE(type) \
typedef struct shtable_ ## type ## _item { \
  char* key; \
  type value; \
} shtable_ ## type ## _item; \
typedef struct shtable_ ## type { \
  shtable_ ## type ## _item * data0; \
  shtable_ ## type ## _item * data1; \
  size_t len; \
} shtable_ ## type; \
shtable_ ## type * shtable_ ## type ## _new(const size_t init_len); \
int shtable_ ## type ## _add(shtable_ ## type * htable, const char* key, const type value); \
int shtable_ ## type ## _get(shtable_ ## type * htable, const char* key, type* value); \
void shtable_ ## type ## _del(shtable_ ## type * htable)

#define SCL_SHTABLE_IMPLEMENT(type) \
shtable_ ## type * shtable_ ## type ## _new(const size_t init_len) { \
  shtable_ ## type * htable = (shtable_ ## type *)malloc(sizeof(shtable_ ## type )); \
  if(!htable) return NULL; \
  htable->len = init_len; \
  htable->data0 = (shtable_ ## type ## _item *)calloc(init_len * 2, sizeof(shtable_ ## type ## _item)); \
  if(!htable->data0) { free(htable); return NULL; } \
  htable->data1 = htable->data0 + init_len; \
  return htable; \
} \
static inline int shtable_ ## type ## _resize(shtable_ ## type * htable, const size_t len) { \
  if(len < htable->len) return 0; \
  shtable_ ## type * nht = shtable_ ## type ## _new(len); \
  if(!nht) return 0; \
  for(size_t i = 0; i < htable->len * 2; i++) if(htable->data0[i].key) \
    shtable_ ## type ## _add(nht, htable->data0[i].key, htable->data0[i].value); \
  size_t tl = htable->len; \
  htable->len = len; \
  nht->len = tl; \
  shtable_ ## type ## _item* d = htable->data0; \
  htable->data0 = nht->data0; \
  htable->data1 = nht->data1; \
  nht->data0 = d; \
  shtable_ ## type ## _del(nht); \
  return 1; \
} \
int shtable_ ## type ## _add(shtable_ ## type * htable, const char* key, const type value) { \
  if(shtable_ ## type ## _get(htable, key, NULL)) return 0; \
  const uint32_t len = strlen(key); \
  const uint32_t y = scl_yoshimura(key, len); \
  const uint32_t m = scl_murmurhash3x86(key, len, y); \
  const uint32_t yi = y % htable->len; \
  const uint32_t mi = m % htable->len; \
    if(!htable->data0[yi].key) { \
      htable->data0[yi].key = malloc(len + 1 * sizeof(char)); \
      if(!htable->data0[yi].key) return 0; \
      strcpy(htable->data0[yi].key, key); \
      htable->data0[yi].value = value; \
      return 1; \
    } \
    if(!htable->data1[mi].key) { \
      htable->data1[mi].key = malloc(len + 1 * sizeof(char)); \
      if(!htable->data1[mi].key) return 0; \
      strcpy(htable->data1[mi].key, key); \
      htable->data1[mi].value = value; \
      return 1; \
    } \
  if(!shtable_ ## type ## _resize(htable, htable->len * 1.5)) return 0; \
  return shtable_ ## type ## _add(htable, key, value); \
} \
int shtable_ ## type ## _get(shtable_ ## type * htable, const char* key, type* value) { \
  if(!key) return 0; \
  const uint32_t len = strlen(key); \
  const uint32_t y = scl_yoshimura(key, len); \
  const uint32_t m = scl_murmurhash3x86(key, len, y); \
  const uint32_t yi = y % htable->len; \
  const uint32_t mi = m % htable->len; \
  char* td0 = htable->data0[yi].key; \
  char* td1 = htable->data1[mi].key; \
  if(td0) if(strcmp(td0, key) == 0) { \
    if(value) *value = htable->data0[yi].value; \
    return 1; \
  } \
  if(td1) if(strcmp(td1, key) == 0) { \
    if(value) *value = htable->data1[mi].value; \
    return 1; \
  } \
  return 0; \
} \
void shtable_ ## type ## _del(shtable_ ## type * htable) { \
  for(size_t i = 0; i < htable->len * 2; i++) \
    if(htable->data0[i].key) free(htable->data0[i].key); \
  free(htable->data0); \
  free(htable); \
} \

#ifdef __cplusplus
}
#endif
#endif
#ifdef SCL_IMPLEMENTATION

sarr* sarr_new(const size_t len) {
  sarr* array = (sarr*)malloc(sizeof(sarr));
  if(!array) return NULL;
  array->len = len;
  array->data = (void**)calloc(len, sizeof(void*));
  if(!array->data) {
    free(array);
    return NULL;
  }
  return array;
}

void sarr_del(sarr* array) {
  free(array->data);
  free(array);
}

slist* slist_new(const size_t init_cap) {
  slist* list = (slist*)malloc(sizeof(slist));
  if(!list) return NULL;
  list->cap = init_cap;
  list->len = 0;
  list->data = (void**)calloc(init_cap, sizeof(void*));
  if(!list->data) {
    free(list);
    return NULL;
  }
  return list;
}

int slist_add(slist* list, void* element) {
  if(list->len >= list->cap) {
    size_t n_cap = list->cap * 2;
    void** data = (void**)realloc(list->data, n_cap * sizeof(void*));
    if(!data) return 0;
    list->data = data;
    list->cap = n_cap;
  }
  list->data[list->len++] = element;
  return 1;
}

void slist_del(slist* list) {
  free(list->data);
  free(list);
}

#define scl_rotl(x, n) (((x) << (n)) | ((x) >> (32-(n))))
static inline uint32_t scl_murmurhash3x86(const char* str, const uint32_t len, uint32_t seed) {
  const uint8_t* data= (const uint8_t*)str;
  const int nblocks = len/4;
  uint32_t h1 = seed;
  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;
  const uint32_t* blocks = (const uint32_t*)(data+nblocks*4);
  for(int i = -nblocks; i; i++) {
    uint32_t k1 = blocks[i];
    k1 *= c1;
    k1 = scl_rotl(k1,15);
    k1 *= c2;
    h1 ^= k1;
    h1 = scl_rotl(h1,13);
    h1 = h1*5+0xe6546b64;
  }
  const uint8_t* tail = (const uint8_t*)(data+nblocks*4);
  uint32_t k1 = 0;
  switch(len&3) {
    case 3:k1^=tail[2]<<16;
    case 2:k1^=tail[1]<<8;
    case 1:k1^=tail[0]; k1*=c1; k1=scl_rotl(k1,15); k1*=c2; h1^=k1;
  };
  h1^=len;
  h1^=h1>>16;
  h1*=0x85ebca6b;
  h1^=h1>>13;
  h1*=0xc2b2ae35;
  h1^=h1>>16;
  return h1;
}
static inline uint32_t scl_yoshimura(const char *str, const uint32_t len) {
  const uint32_t p = 709607;
  uint32_t h0 = 2166136261, h1 = 2166136261;
  const char* c = str;
  uint32_t i, off;
  if(len >= 2*2*sizeof(uint32_t)){
    off = len-((len>>4)+1)*(2*4);
    i = (len>>4);
    i++;
    for(; i; i--,c+=2*sizeof(uint32_t)) {
      h0 = (h0^(scl_rotl(*(uint32_t*)(c+0),5)^*(uint32_t*)(c+0+off)))*p;
      h1 = (h1^(scl_rotl(*(uint32_t*)(c+4+off),5)^*(uint32_t*)(c+4)))*p;
    }
  } else {
#define scl_ysize(U32T2,U32T1,U16T1,T1,N) (len&2*sizeof(uint32_t))?U32T2: \
(len&sizeof(uint32_t)?U32T1:(len&sizeof(uint16_t)?U16T1:(len&1?T1:N)))
    h0 = scl_ysize((h0^*(uint32_t*)(c+0))*p,(h0^*(uint16_t*)(c+0))*p,
      (h0^*(uint16_t*)c)*p,(h0&*c)*p,h0);
    h1 = scl_ysize((h1^*(uint32_t*)(c+4))*p,(h1^*(uint16_t*)(c+2))*p,h1,h1,h1);
    c += scl_ysize(4*sizeof(uint16_t),
    2*sizeof(uint16_t),sizeof(uint16_t),0,0);
  }
  h0 = (h0^scl_rotl(h1,5))*p;
  return h0^(h0>>16);
}
#undef scl_ysize
#undef scl_rotl

shtable* shtable_new(const size_t init_len) {
  shtable* htable = (shtable*)malloc(sizeof(shtable));
  if(!htable) return NULL;
  htable->len = init_len;
  htable->data0 = (shtable_item*)calloc(init_len * 2, sizeof(shtable_item));
  if(!htable->data0) { free(htable); return NULL; }
  htable->data1 = htable->data0 + init_len;
  return htable;
}

static inline int shtable_resize(shtable* htable, const size_t len) {
  if(len < htable->len) return 0;
  shtable* nht = shtable_new(len);
  if(!nht) return 0;
  for(size_t i = 0; i < htable->len * 2; i++) if(htable->data0[i].key)
    shtable_add(nht, htable->data0[i].key, htable->data0[i].value);
  size_t tl = htable->len;
  htable->len = len;
  nht->len = tl;
  shtable_item* d = htable->data0;
  htable->data0 = nht->data0;
  htable->data1 = nht->data1;
  nht->data0 = d;
  shtable_del(nht);
  return 1;
}

int shtable_add(shtable* htable, const char* key, const void* value) {
  if(shtable_get(htable, key, NULL)) return 0;
  const uint32_t len = strlen(key);
  const uint32_t y = scl_yoshimura(key, len);
  const uint32_t m = scl_murmurhash3x86(key, len, y);
  const uint32_t yi = y % htable->len;
  const uint32_t mi = m % htable->len;
    if(!htable->data0[yi].key) {
      htable->data0[yi].key = (char*)malloc(len + 1 * sizeof(char));
      if(!htable->data0[yi].key) return 0;
      strcpy(htable->data0[yi].key, key);
      htable->data0[yi].value = (void*)value;
      return 1;
    }
    if(!htable->data1[mi].key) {
      htable->data1[mi].key = (char*)malloc(len + 1 * sizeof(char));
      if(!htable->data1[mi].key) return 0;
      strcpy(htable->data1[mi].key, key);
      htable->data1[mi].value = (void*)value;
      return 1;
    }
  if(!shtable_resize(htable, htable->len * 1.5)) return 0;
  return shtable_add(htable, key, value);
}

int shtable_get(shtable* htable, const char* key, void** value) {
  if(!key) return 0;
  const uint32_t len = strlen(key);
  const uint32_t y = scl_yoshimura(key, len);
  const uint32_t m = scl_murmurhash3x86(key, len, y);
  const uint32_t yi = y % htable->len;
  const uint32_t mi = m % htable->len;
  char* td0 = htable->data0[yi].key;
  char* td1 = htable->data1[mi].key;
  if(td0) if(strcmp(td0, key) == 0) {
    if(value) *value = htable->data0[yi].value;
    return 1;
  }
  if(td1) if(strcmp(td1, key) == 0) {
    if(value) *value = htable->data1[mi].value;
    return 1;
  }
  return 0;
}

void shtable_del(shtable* htable) {
  for(size_t i = 0; i < htable->len * 2; i++)
    if(htable->data0[i].key) free(htable->data0[i].key);
  free(htable->data0);
  free(htable);
}

char* sread(const char* filepath, int nul_terminate, size_t* size) {
    FILE* file = fopen(filepath, "rb");
    if(!file) return NULL;
    fseek(file, 0L, SEEK_END);
    long f_size = ftell(file);
    *size = f_size;
    rewind(file);
    long m_size = f_size;
    if(nul_terminate) ++m_size;
    char* content = (char*)malloc(m_size);
    if(!content) {
        fclose(file);
	return NULL;
    }
    if(fread(content, f_size, 1, file) != 1) {
        fclose(file);
	free(content);
	return NULL;
    }
    fclose(file);
    if(nul_terminate) content[f_size] = '\0';
    return content;
}

char** sreadlns(const char* filepath, size_t* len) {
    size_t size = 0;
    char* content = sread(filepath, 1, &size);
    if(!content) return NULL;
    char** array = (char**)malloc((size+1)*sizeof(char*));
    if(!array) {
	free(content);
	return NULL;
    }
    char* line = strtok(content, "\r\n");
    size_t i = 0;
    for(; line; i++) {
	size_t l = strlen(line)+1;
	array[i] = (char*)malloc(l);
	memcpy(array[i], line, l);
        line = strtok(NULL, "\r\n");
    }
    *len = i;
    return array;
}

void* smalloc(size_t size, const char* file, int line, const char* fn) {
  void* p = malloc(size);
  fprintf(stderr, "[smalloc] %p[%zu] %s:%u %s()\n", p, size, file, line, fn);
  return p;
}

void* scalloc(size_t n, size_t size, const char* file, int line, const char* fn) {
  void* p = calloc(n, size);
  fprintf(stderr, "[smalloc] %p[%zu] %s:%u %s()\n", p, n*size, file, line, fn);
  return p;
}

void sfree(void* e, const char* file, int line, const char* fn) {
  fprintf(stderr, "[smalloc] %p[free] %s:%i %s()\n", e, file, line, fn);
  free(e);
}

#endif

//
// This software can be used under one of two licenses.
// Choose whichever you prefer.
// 
// OPTION A: MIT License
// Copyright (c) 2024 Mariusz Krzyżok
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// OPTION B: Public Domain (unlicense.org)
// This is free and unencumbered software released into the public domain.
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// For more information, please refer to <https://unlicense.org>
//
