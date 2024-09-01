cc=gcc
FLAGS=-w

grades : commands.h commands.c list.h list.c openfile.h openfile.c main.c
						$(cc) $(FLAGS) $^ -o $@

.PHONY: clear

clear:
						rm grades