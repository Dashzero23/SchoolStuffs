# Assignment 2
This program implements different formulas to estimate Euler's number and Pi value.

## Build
```
$ make
```
## Run
In order to run the program you need to specify which test to run.
 * -a : Runs all tests.
 * -e : Runs e approximation test.
 * -b : Runs Bailey-Borwein-Plouffe π approximation test.
 * -m : Runs Madhava π approximation test.
 * -r : Runs Euler sequence π approximation test.
 * -v : Runs Viète π approximation test.
 * -n : Runs Newton-Raphson square root approximation tests.
 * -s : Enable printing of statistics to see computed terms and factors for each tested function.
 * -h : Display a help message detailing program usage.

```
$ ./mathlib-test -a -s
```
The code above runs all test with computed terms and factors for each tested function
## Cleaning
```
$ make clean
```
## Files
### bbp.c
This contains the implementation of the Bailey-Borwein-Plouffe formula to approximate π and the function to return the number of computed terms.
### e.c
This contains the implementation of the Taylor series to approximate Euler’s number e and the function to return the number of computed terms.
### euler.c
This contains the implementation of Euler’s solution used to approximate π and the function to return the number of computed terms.
###  madhava.c
This contains the implementation of the Madhava series to approximate π and the function to return the number of computed terms.
###  mathlib-test.c
This contains the main() function which tests each of my math library functions.
###  mathlib.h
This contains the interface for my math library.
###  newton.c
This contains the implementation of the square root approximation using Newton’s method and the function to return the number of computed iterations.
###  viete.c 
This contains the implementation of Viète’s formula to approximate π and the function to return the number of computed factors.
### Makefile
Makefile has all the commands to compile and clean the files
### DESIGN.pdf
This file has information about the design of all the implementation as well as mathlib-test.c. This also gives the idea on how to run the program.
### WRITEUP.pdf
Include the plots that is produced using the bash script, as well as discussion on the difference between the approximated values and the actual values.