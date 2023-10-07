CC = clang
CFLAGS = -Wstrict-prototypes -Wall -Wextra -Werror -Wpedantic
LFLAGS = $(shell pkg-config --libs gmp)
EXEC = encode decode
SOURCES =  trie.c io.c word.c
OBJECTS = $(SOURCES:%.c=%.o)

all: $(EXEC)

encode: $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -g -o encode $(OBJECTS) encode.c -lm

decode: $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -g -o decode $(OBJECTS) decode.c -lm

%.o:%.c
	$(CC) $(CFLAGS) -c $<

format: 
	clang-format -i -style=file *.[ch]

clean:
	rm -rf encode decode $(OBJECTS)

scan-build: clean
	scan-build make

