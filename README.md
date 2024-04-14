# small c library

## goals
- be small (in code and size)
- allocate on heap as least as possible
- be fast
- be safe and sanitized

## contents

### net

- [sockets.h](net/sockets.h) - send and recv wrappers, client and server boilerplate

### print

- [color.h](print/color.h) - ansi colors macros
- [spin.h](print/spin.h) - progress spinner

### data

- [array.h](data/array.h) - dynamic array
