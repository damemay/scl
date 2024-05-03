# small c library

## goals
- small, efficient, sanitized code with api for humans.
- allocate on heap inside lib only when necessary.
- implement things of interest.

## contents

### net

- [socket.h](net/socket.h) - send and recv wrappers, client and server boilerplate
- [http.h](net/http.h) - fast http client with simple api and headers parsing

### data

- [array.h](data/array.h) - dynamic array
- [map.h](data/map.h) - array map based on dynamic arrays for small data
- [hmap.h](data/hmap.h) - hash map

### print

- [color.h](print/color.h) - ansi colors macros
- [spin.h](print/spin.h) - progress spinner

### math

- [hash.h](math/hash.h) - string hashing functions
- [prime.h](math/prime.h) - prime number finding functions
