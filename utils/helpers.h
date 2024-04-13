#ifndef __SCL_HELPERS_H__
#define __SCL_HELPERS_H__

#ifndef NDEBUG
#define scl_verr(FMT, ...)  fprintf(stderr, "scl:err:" FMT "!\n", __VA_ARGS__)
#define scl_err(MSG)        fprintf(stderr, "scl:err:" MSG "!\n")
#define scl_vlog(FMT, ...)  printf("scl:log:" FMT "\n", __VA_ARGS__)
#define scl_log(MSG)        printf("scl:log:" MSG "\n")
#else
#define scl_verr(FMT, ...)  do{}while(0)
#define scl_err(MSG)        do{}while(0)
#define scl_vlog(FMT, ...)  do{}while(0)
#define scl_log(MSG)        do{}while(0)
#endif

#endif
