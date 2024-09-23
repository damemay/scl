# smol c library

## goals
- smol, fast and sanitized code with api for humans
- implement basic things of interest for quick reusage in projects
- one header file, one source file
- public domain license

## contents

- `sarr` - generic-type dynamic array
- `sdic` - generic-type value dynamic dictionary with [yoshimura hash](http://www.sanmayce.com/Fastest_Hash/) and nul-terminated char* keys
- `sread` and `sreadlns` - read file into char* (nul-terminated or not) or into char* array
