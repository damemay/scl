#include <string.h>
#include <math.h>
#include "../math/hash.h"

int scl_hash(const char* string, const int prime, const int buckets) {
    long hash = 0;
    const int len_s = strlen(string);
    for(size_t i=0; i<len_s; i++) {
	hash += (long)pow(prime, len_s - (i+1)) * string[i];
	hash %= buckets;
    }
    return (int)hash;
}

int scl_hash_get(const char* string, const int buckets, const int attempt) {
    const int a = scl_hash(string, 151, buckets);
    const int b = scl_hash(string, 191, buckets);
    return (a + (attempt*(b+1))) % buckets;
}
