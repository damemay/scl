#include <stdint.h>
#include <string.h>
#include <stdio.h>
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

static inline uint32_t yoshimura(const char *str, uint32_t len) {
#define SCL_ROTL(x, n) (((x) << (n)) | ((x) >> (32-(n))))
    const uint32_t p = 709607;
    uint32_t h0 = 2166136261, h1 = 2166136261;
    const char *c = str;
    uint32_t i, off;
    if (len >= 2*2*sizeof(uint32_t)) {
        off = len-((len>>4)+1)*(2*4); i = (len>>4); i++;
        for(; i; i--, c += 2*sizeof(uint32_t)) {
	    h0 = (h0 ^ (SCL_ROTL(*(uint32_t *)(c+0),5) ^ *(uint32_t *)(c+0+off))) * p;        
	    h1 = (h1 ^ (SCL_ROTL(*(uint32_t *)(c+4+off),5) ^ *(uint32_t *)(c+4))) * p;        
        }
    } else {
#define SIZE(U32T2, U32T1, U16T1, T1, N) (len & 2*sizeof(uint32_t)) ? U32T2 : (len & sizeof(uint32_t) ? U32T1 : (len & sizeof(uint16_t) ? U16T1 : (len & 1 ? T1 : N)))
	h0 = SIZE((h0 ^ *(uint32_t*)(c+0)) * p, (h0 ^ *(uint16_t*)(c+0)) * p, (h0 ^ *(uint16_t*)c) * p, (h0 & *c) * p, h0);
	h1 = SIZE((h1 ^ *(uint32_t*)(c+4)) * p, (h1 ^ *(uint16_t*)(c+2)) * p, h1, h1, h1);
	c += SIZE(4*sizeof(uint16_t), 2*sizeof(uint16_t), sizeof(uint16_t), 0, 0);
    }
    h0 = (h0 ^ SCL_ROTL(h1,5)) * p;
    return h0 ^ (h0 >> 16);
#undef SIZE
#undef SCL_ROTL
}

scl_dic* scl_dic_new() {
    scl_dic* dict = scl_arr_new(sizeof(struct scl_di*));
    if(!dict) return NULL;
    return dict;
}

int scl_dic_add(scl_dic* dict, const char* key, const void* value) {
    size_t key_len = strlen(key);
    uint32_t hash = yoshimura(key, key_len);
    for(size_t i=0; i<dict->len; i++) {
	struct scl_di* it = (struct scl_di*)dict->data[i];
	if(yoshimura(it->key, strlen(it->key)) == hash) return -1;
    }
    struct scl_di* item = malloc(sizeof(struct scl_di));
    if(!item) return -1;
    item->key = malloc(key_len+1);
    if(!item->key) {
	free(item);
	return -1;
    }
    memcpy(item->key, key, key_len+1);
    item->value = (void*)value;
    return scl_arr_add(dict, item);
}

void* scl_dic_get(scl_dic* dict, const char* key) {
    uint32_t hash = yoshimura(key, strlen(key));
    for(size_t i=0; i<dict->len; i++) {
	struct scl_di* it = (struct scl_di*)dict->data[i];
	if(yoshimura(it->key, strlen(it->key)) == hash) return it->value;
    }
    return NULL;
}

void scl_dic_free(scl_dic* dict) {
    for(size_t i=0; i<dict->len; i++) {
	struct scl_di* item = (struct scl_di*)dict->data[i];
	free(item->key);
	free(item);
    }
    scl_arr_free(dict);
}

char* scl_rd(const char* filepath, int nul_terminate, size_t* size) {
    FILE* file = fopen(filepath, "rb");
    if(!file) return NULL;
    fseek(file, 0L, SEEK_END);
    long f_size = ftell(file);
    *size = f_size;
    rewind(file);
    long m_size = f_size;
    if(nul_terminate) ++m_size;
    char* content = malloc(m_size);
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

char** scl_rdl(const char* filepath, size_t* len) {
    size_t size = 0;
    char* content = scl_rd(filepath, 1, &size);
    if(!content) return NULL;
    char** array = malloc(size+1);
    if(!array) {
	free(content);
	return NULL;
    }
    char* line = strtok(content, "\r\n");
    size_t i = 0;
    for(; line; i++) {
	size_t l = strlen(line)+1;
	array[i] = malloc(l);
	memcpy(array[i], line, l);
        line = strtok(NULL, "\r\n");
    }
    *len = i;
    return array;
}
