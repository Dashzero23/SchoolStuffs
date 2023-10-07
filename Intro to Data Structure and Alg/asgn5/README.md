# Assignment 5
This programs implement the Schmidt-Samoa (SS) encryption algorithm

## Build
```
$ make
```
In order to build them separately, you can append either 'keygen', 'encrypt', or 'decrypt'.

## Run keygen
-b : specifies the minimum bits needed for the public modulus n.
-i : specifies the number of Miller-Rabin iterations for testing primes (default: 50).
-n pbfile : specifies the public key file (default: ss.pub).
-d pvfile : specifies the private key file (default: ss.priv).
-s : specifies the random seed for the random state initialization (default: the seconds since the UNIX epoch, given by time(NULL)).
-v : enables verbose output.
-h : displays program synopsis and usage.

```
$ ./keygen
```
## Run encrypt
-i : specifies the input file to encrypt (default: stdin).
-o : specifies the output file to encrypt (default: stdout).
-n : specifies the file containing the public key (default: ss.pub).
-v : enables verbose output.
-h : displays program synopsis and usage.
```
$ ./encrypt -i [original input file] -o [encrypted output file]
```
## Run decrypt
-i : specifies the input file to encrypt (default: stdin).
-o : specifies the output file to encrypt (default: stdout).
-n : specifies the file containing the public key (default: ss.priv).
-v : enables verbose output.
-h : displays program synopsis and usage.
```
$ ./decrypt -i [crypted input file] -o [decrypted output file]
```
## Cleaning
```
$ make clean
```
## Files
### randstate.(c,h)
This contains the implementation/header of the randstate variable.
### numtheory.(c.h)
This contains the implementation/header of the number theories that is used in the code
### ss.(c.h)
This contains the implementation/header of the Schmidt-Samoa algorithm
### keygen.c
This contains the main function for keygen
### encrypt.c
This contains the main function for encrypt
### decrypt.c
This contains the main function for decrypt
### Makefile
Makefile has all the commands to compile and clean the files
### DESIGN.pdf
This file has information about the design of all the functions and thought process on making them. This also gives the idea on how to run the program.
### WRITEUP.pdf
Include everything I learned while doing this assignment