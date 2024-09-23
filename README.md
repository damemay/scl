# smol c library

## goals
- smol, efficient, fast and sanitized code with api for humans
- implement basic things of interest for quick reusage in projects
- one header file, one source file
- public domain license

## contents

- `scl_arr` - generic-type dynamic array
- `scl_dic` - generic-type value dynamic dictionary with [yoshimura hash](http://www.sanmayce.com/Fastest_Hash/) and nul-terminated char* keys
- `scl_rd` and `scl_rdl` - read file into char* (nul-terminated or not) or into char* array
