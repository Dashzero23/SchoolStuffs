SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
OBJS = e.o madhava.o euler.o bbp.o viete.o newton.o #with help from multiple TA
EXEC = e madhava euler bbp viete newton #with help from multiple TA

all: mathlib-test

mathlib-test: mathlib-test.o $(OBJS)
	$(CC) -o mathlib-test mathlib-test.o $(OBJS) -lm

mathlib-test.o: mathlib-test.c
	$(CC) $(CFLAGS) -c mathlib-test.c

clean:
	rm -f mathlib-test *.o $(OBJS)

format:
	clang-format -i -style=file *.[ch]

%: %.o
	$(CC) -o $(EXEC) $(OBJS) #with help from multiple TA

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ #with help from multiple TA

