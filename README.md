# smol c library

## goals

- fast, efficient code
- small, easy to use api
- use C99 for compatibility
- tested and sanitized with UBSan and ASan
- implement basic things of interest for quick reusage in C projects
- single header file
- license for you to choose: public domain or MIT 

## contents

### data structures
- `sarr_ptr` - `void*` array
- `slist_ptr` - `void*` dynamic list
- `shtable_ptr` - `void*` dynamic hash table
    - simplified implementation of cuckoo hashing (no loop)
    - hashed with [yoshimura](http://www.sanmayce.com/Fastest_Hash/) and [MurmurHash3](https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp)
    - uses nul-terminated char* keys

all of the structures above can be generated for non-pointer values with `SCL_STRUCTURE_DEFINE(type)` and `SCL_STRUCTURE_IMPLEMENT(type)` macros. Refer to [scl.h](scl.h) for guidance.

### utility functions
- `sread` and `sreadlns` - read file into char* (nul-terminated or not) or into char* array
- `smalloc`, `scalloc`, `sfree` - 
