CC=clang
CFLAGS=-Wstrict-prototypes -Wall -Wextra -Werror -Wpedantic
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: sorting

sorting: $(OBJS)
	clang -o $@ $^ -lm

%.o: %.c
	clang $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) sorting
format:
	clang-format -i -style=file *.[ch]
#done with the help of multiple TAs/tutors
