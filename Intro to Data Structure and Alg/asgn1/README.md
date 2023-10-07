# Assignment 1
This program uses code monte_carlo.c to plot 2 different plots, Monte Carlo estimation of pi, and Monte Carlo error estimation. The script plot.sh can be modified to generate different seed or number of iterations of the program monte_carlo.c. The Makefile file has many commands that can be used to save times in creating and clearing sincos or monte_carlo executable file.

## Build
$ make
## Run
$ ./plot.sh
## Cleaning
$ make clean
## Files
### ./plot.sh
The main file which runs and create the plots and data.dat files.
### monte_carlo.c
The main programmed file, used to create the plots and data.dat files.
### Makefile
This file has commands which are used to save times and allowed to use new commands in the terminal.
### DESIGN.pdf
This file has information about the design of plot.sh. This also gives the idea on how to run the program.
### WRITEUP.pdf
Include the plots that is produced using the bash script, as well as discussion on which UNIX commands I used to produce each plot and why I chose to use them.