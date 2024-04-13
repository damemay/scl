#ifndef __SECL_HELPERS_H__
#define __SECL_HELPERS_H__

#ifndef NDEBUG
#define secl_verr(FMT, ...)  fprintf(stderr, "secl:err:" FMT "!\n", __VA_ARGS__)
#define secl_err(MSG)        fprintf(stderr, "secl:err:" MSG "!\n")
#define secl_vlog(FMT, ...)  printf("secl:log:" FMT "\n", __VA_ARGS__)
#define secl_log(MSG)        printf("secl:log:" MSG "\n")
#else
#define secl_verr(FMT, ...)  do{}while(0)
#define secl_err(MSG)        do{}while(0)
#define secl_vlog(FMT, ...)  do{}while(0)
#define secl_log(MSG)        do{}while(0)
#endif

#endif
