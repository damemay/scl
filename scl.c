#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "scl.h"

sarr* sarr_new() {
    sarr* array = malloc(sizeof(sarr));
    if(!array) return NULL;
    array->len = 0;
    array->cap = 16;
    array->data = calloc(16, sizeof(void*));
    if(!array->data) {
	free(array);
	return NULL;
    }
    return array;
}

int sarr_add(sarr* array, const void* element) {
    if(array->len >= array->cap) {
	size_t n_cap = array->cap * 2;
	void** data = realloc(array->data, n_cap * sizeof(void*));
	if(!data) return -1;
	array->data = data;
	array->cap = n_cap;
    }
    array->data[array->len++] = (void*)element;
    return 0;
}

void* sarr_get(sarr* array, size_t index) {
    if(index >= array->len || index < 0) return NULL;
    return array->data[index];
}

void sarr_free(sarr* array) {
    free(array->data);
    free(array);
}

static inline uint32_t yoshimura(const char *str, uint32_t len) {
#define SDIC_ROTL(x, n) (((x) << (n)) | ((x) >> (32-(n))))
    const uint32_t p = 709607;
    uint32_t h0 = 2166136261, h1 = 2166136261;
    const char *c = str;
    uint32_t i, off;
    if (len >= 2*2*sizeof(uint32_t)) {
        off = len-((len>>4)+1)*(2*4); i = (len>>4); i++;
        for(; i; i--, c += 2*sizeof(uint32_t)) {
	    h0 = (h0 ^ (SDIC_ROTL(*(uint32_t *)(c+0),5) ^ *(uint32_t *)(c+0+off))) * p;        
	    h1 = (h1 ^ (SDIC_ROTL(*(uint32_t *)(c+4+off),5) ^ *(uint32_t *)(c+4))) * p;        
        }
    } else {
#define SDIC_SIZE(U32T2, U32T1, U16T1, T1, N) (len & 2*sizeof(uint32_t)) ? U32T2 : (len & sizeof(uint32_t) ? U32T1 : (len & sizeof(uint16_t) ? U16T1 : (len & 1 ? T1 : N)))
	h0 = SDIC_SIZE((h0 ^ *(uint32_t*)(c+0)) * p, (h0 ^ *(uint16_t*)(c+0)) * p, (h0 ^ *(uint16_t*)c) * p, (h0 & *c) * p, h0);
	h1 = SDIC_SIZE((h1 ^ *(uint32_t*)(c+4)) * p, (h1 ^ *(uint16_t*)(c+2)) * p, h1, h1, h1);
	c += SDIC_SIZE(4*sizeof(uint16_t), 2*sizeof(uint16_t), sizeof(uint16_t), 0, 0);
    }
    h0 = (h0 ^ SDIC_ROTL(h1,5)) * p;
    return h0 ^ (h0 >> 16);
#undef SDIC_SIZE
#undef SDIC_ROTL
}

sdic* sdic_new() {
    sdic* dict = sarr_new();
    if(!dict) return NULL;
    return dict;
}

int sdic_add(sdic* dict, const char* key, const void* value) {
    size_t key_len = strlen(key);
    uint32_t hash = yoshimura(key, key_len);
    for(size_t i=0; i<dict->len; i++) {
	struct sdic_i* it = (struct sdic_i*)dict->data[i];
	if(yoshimura(it->key, strlen(it->key)) == hash) return -1;
    }
    struct sdic_i* item = malloc(sizeof(struct sdic_i));
    if(!item) return -1;
    item->key = malloc(key_len+1);
    if(!item->key) {
	free(item);
	return -1;
    }
    memcpy(item->key, key, key_len+1);
    item->value = (void*)value;
    return sarr_add(dict, item);
}

void* sdic_get(sdic* dict, const char* key) {
    uint32_t hash = yoshimura(key, strlen(key));
    for(size_t i=0; i<dict->len; i++) {
	struct sdic_i* it = (struct sdic_i*)dict->data[i];
	if(yoshimura(it->key, strlen(it->key)) == hash) return it->value;
    }
    return NULL;
}

void sdic_free(sdic* dict) {
    for(size_t i=0; i<dict->len; i++) {
	struct sdic_i* item = (struct sdic_i*)dict->data[i];
	free(item->key);
	free(item);
    }
    sarr_free(dict);
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

char** sreadlns(const char* filepath, size_t* len) {
    size_t size = 0;
    char* content = sread(filepath, 1, &size);
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
