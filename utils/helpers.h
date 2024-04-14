#ifndef __SCL_HELPERS_H__
#define __SCL_HELPERS_H__
#include <stdio.h>

#ifndef NDEBUG
#define SCL_VERR(FMT, ...)  fprintf(stderr, "scl:err:" FMT "!\n", __VA_ARGS__)
#define SCL_ERR(MSG)        fprintf(stderr, "scl:err:" MSG "!\n")
#define SCL_VLOG(FMT, ...)  printf("scl:log:" FMT "\n", __VA_ARGS__)
#define SCL_LOG(MSG)        printf("scl:log:" MSG "\n")
#else
#define SCL_VERR(FMT, ...)  do{}while(0)
#define SCL_ERR(MSG)        do{}while(0)
#define SCL_VLOG(FMT, ...)  do{}while(0)
#define SCL_LOG(MSG)        do{}while(0)
#endif

#endif
