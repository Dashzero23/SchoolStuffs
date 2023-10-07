# Assignment 3
This program implements different sorting algorithms

## Build
```
$ make
```
## Run
In order to run the program you need to specify which test to run.
 * -H : Display a help message detailing program usage.
 * -a : Runs all tests.
 * -h : Enable Heap Sort.
 * -b : Enable Batcher Sort.
 * -s : Enable Shell Sort.
 * -q : Enable Quick Sort.
 * -r [num] : sets seed for random array. Default is 13371453.
 * -n [num]: changes the size of the array. Default is 100.
 * -p [num]: sets how many elements of the array to print. Default is 100.

```
$ ./sorting -a
```
The code above runs all sorts with the default setting
## Cleaning
```
$ make clean
```
## Files
### shell.(c,h)
This contains the implementation/header of Shell sort.
### batcher.(c,h)
This contains the implementation/header of Batcher sort.
### quick.(c,h)
This contains the implementation/header of Quick sort.
###  heap.(c,h)
This contains the implementation/header of Heap sort.
###  sorting.c
This contains the main() function which tests each of the sorting algorithm.
###  gaps.h
This contains the gap sequence for Shell sort.
###  set.(c,h)
This contains the implementation of the 'set' to track the which sort to use.
###  stats.(c,h) 
This contains the implementation of a type Struct variable to keep track of the data of each sort.
### Makefile
Makefile has all the commands to compile and clean the files
### DESIGN.pdf
This file has information about the design of all the implementation as well as sorting.c. This also gives the idea on how to run the program.
### WRITEUP.pdf
Include the plots that is produced using the bash script, as well as discussion on the performance of each.