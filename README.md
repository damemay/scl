# small c library

## goals
- small, efficient, sanitized code with api for humans.
- allocate on heap inside lib only when necessary.
- learn and implement data structures, algorithms and other things of interest.

## contents

### net

- [socket.h](net/socket.h) - send and recv wrappers, client and server boilerplate
- [http.h](net/http.h) - fast http client with simple api and headers parsing

### print

- [color.h](print/color.h) - ansi colors macros
- [spin.h](print/spin.h) - progress spinner

### data

- [array.h](data/array.h) - dynamic array
- [map.h](data/map.h) - array map based on dynamic arrays
