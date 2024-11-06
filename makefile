CC=gcc
STD=-std=c99
FLAGS=-Wall -Werror -pedantic \
      -g -fno-omit-frame-pointer \
      -fsanitize=undefined \
      -fsanitize=float-divide-by-zero \
      -fsanitize=address
% : %.c
	$(CC) $(STD) $(FLAGS) $< -o $@
