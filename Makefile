CC = /usr/local/Cellar/gcc/13.2.0/bin/gcc-13
RELEASE_FLAGS = -DNDEBUG -O2
DEBUG_FLAGS = -g
SANITIZE_FLAGS = -g -fsanitize=address -O1 -fno-omit-frame-pointer

SOURCES = $(wildcard src/*.c)
OBJECTS := $(patsubst %.c,%.o,$(wildcard src/*.c))
EXECUTABLES := $(patsubst %.c,%,$(wildcard tests/*.c))

tests: CFLAGS = $(DEBUG_FLAGS)
tests: LDFLAGS = $(wildcard build/*.o)
tests-sanitize: CFLAGS = $(SANITIZE_FLAGS)
tests-sanitize: LDFLAGS = $(wildcard build/*.o)
lib: CFLAGS = $(RELEASE_FLAGS)

.PHONY: clean tests

source: $(OBJECTS)
	mv src/*.o build/

lib: source
	ar rcs libcls.a build/*.o

test: $(EXECUTABLES)

tests: source test

tests-sanitize: source test

clean:
	rm build/*
