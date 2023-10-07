# Assignment 6
This programs implement the LZ78 compression algorithm

## Build
```
$ make
```
In order to build them separately, you can append either 'encode' or 'decode'.

## Run encode
-i (input file): file being compressed.
-o (output file): the compressed file.
-v : print verbose/statistic.


```
$ ./encode -i (input file) -o (output file) -v
```
## Run decode
-i (input file): file being decompressed.
-o (output file): the decompressed file.
-v : print verbose/statistic.
```
$ ./decode -i (input file) -o (output file) -v
```

## Cleaning
```
$ make clean
```
## Files
### trie.(c,h)
This contains the implementation/header of the trie ADT.
### word.(c.h)
This contains the implementation/header of the word/word table ADT
### io.(c.h)
This contains the implementation/header of the I/O buffer
### code.h
This contains the header of all needed code to use in the program
### endian.h
This contains the header for checking and swaping endian value
### encode.c
This contains the main function for encode
### decode.c
This contains the main function for decode
### Makefile
Makefile has all the commands to compile and clean the files
### DESIGN.pdf
This file has information about the design of all the functions and thought process on making them. This also gives the idea on how to run the program.
### WRITEUP.pdf
Include everything I learned while doing this assignment