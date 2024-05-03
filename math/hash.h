#ifndef __SCL_HASH_H__
#define __SCL_HASH_H__

int scl_hash(const char* string, const int prime, const int buckets);
int scl_hash_get(const char* string, const int buckets, const int attempt);

#endif
