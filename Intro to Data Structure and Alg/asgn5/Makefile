CC = clang
CFLAGS = -Wstrict-prototypes -Wall -Wextra -Werror -Wpedantic
LFLAGS = $(shell pkg-config --libs gmp)
SOURCES = randstate.c numtheory.c ss.c
EXEC = test
OBJECTS = $(SOURCES:%.c=%.o)

all: keygen encrypt decrypt

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -g -o keygen $(SOURCES) keygen.c
	$(CC) $(CFLAGS) $(LFLAGS) -g -o encrypt $(SOURCES) encrypt.c
	$(CC) $(CFLAGS) $(LFLAGS) -g -o decrypt $(SOURCES) decrypt.c


%.o:%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJECTS) keygen encrypt decrypt

format: 
	clang-format -i -style=file *.[c]

keygen:
	$(CC) $(CFLAGS) $(LFLAGS) -lm -g -o keygen ss.c numtheory.c randstate.c keygen.c

encrypt:
	$(CC) $(CFLAGS) $(LFLAGS) -lm -g -o encrypt ss.c numtheory.c randstate.c encrypt.c

decrypt: 
	$(CC) $(CFLAGS) $(LFLAGS) -lm -g -o decrypt ss.c numtheory.c randstate.c decrypt.c

pkg-config:
	sudo apt install pkg-config libgmp-dev
	
scan-build: clean
	scan-build --use-cc=clang -no-failure-reports --status-bugs make
