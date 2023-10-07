CC = clang
CFLAGS= -Wstrict-prototypes -Wall -Wextra -Werror -Wpedantic
LFLAGS = -lncurses
SOURCES = $(wildcard *.c)
EXEC = life
OBJECTS = $(SOURCES:%.c=%.o)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) $(OBJECTS)

format:
	clang-format -i -style=file *.[c,h]
