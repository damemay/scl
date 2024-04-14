CC = gcc
RELEASE_FLAGS = -DNDEBUG -O2
TEST_FLAGS = 

SOURCES = $(wildcard src/*.c)
OBJECTS := $(patsubst %.c,%.o,$(wildcard src/*.c))
EXECUTABLES := $(patsubst %.c,%,$(wildcard tests/*.c))

CFLAGS = $(TEST_FLAGS)
LDFLAGS = $(wildcard build/*.o)

.PHONY: clean

source: $(OBJECTS)
	mv src/*.o build/

test: $(EXECUTABLES)
	find tests/ -type f | grep -v "\.c" | xargs -I@ mv @ build/

clean:
	rm build/*
