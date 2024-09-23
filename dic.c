#include <string.h>
#include "scl.h"

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
