CC = /usr/local/Cellar/gcc/13.2.0/bin/gcc-13
RELEASE_FLAGS = -DNDEBUG -O2
TEST_FLAGS = -g -fsanitize=address -O1 -fno-omit-frame-pointer
#TEST_FLAGS = -g 

SOURCES = $(wildcard src/*.c)
OBJECTS := $(patsubst %.c,%.o,$(wildcard src/*.c))
EXECUTABLES := $(patsubst %.c,%,$(wildcard tests/*.c))

CFLAGS = $(TEST_FLAGS)
LDFLAGS = $(wildcard build/*.o)

.PHONY: clean tests

source: $(OBJECTS)
	mv src/*.o build/

test: $(EXECUTABLES)

tests: source test

clean:
	rm build/*
