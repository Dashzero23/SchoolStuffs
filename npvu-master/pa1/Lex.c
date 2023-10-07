/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Winter CSE101 PA1
* Lex.h
* Main program for the assignment
*********************************************************************************/
// check command line for correct number of arguments, open read/write files, count input file lines are from the given example FileIO.c file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

#define MAX_LEN 300

int main(int argc, char * argv[]) {
    FILE *in, *out;
    char line[MAX_LEN];

    // check command line for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // open files for reading and writing
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    if (in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    if (out == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }

    int lines = 0; 

    while (fgets(line, MAX_LEN, in) != NULL) { // count total number of lines
        lines++;
    }

    rewind(in); // go back to the beginning of the file

    char **array = (char **) calloc(lines, sizeof(char *)); // allocate memory for the array of strings

    if (array == NULL) { // check for memory allocation error
        fprintf(stderr, "Error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // put the data from the in file to the array
    for (int i = 0; i < lines; i++) { 
        if (fgets(line, MAX_LEN, in) == NULL) { // read in the lines of the file as strings
            fprintf(stderr, "Error: could not read line %d.\n", i + 1);
            exit(EXIT_FAILURE);
        }

        array[i] = (char *) calloc(strlen(line) + 1, sizeof(char)); // allocate memory for the string

        if (array[i] == NULL) { // check for memory allocation error
            fprintf(stderr, "Error: memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        // copy the string into the array
        strncpy(array[i], line, strlen(line) + 1);
    }
    
    // strcmp(s1, s2)<0 is true if and only if s1 comes before s2
    // strcmp(s1, s2)>0 is true if and only if s1 comes after s2
    // strcmp(s1, s2)==0 is true if and only if s1 is identical to s2
    // in my way of implementing, I only care about whether strcmp(s1, s2)>0 is true
    // insertion sort
    // I just realized I need to make a list and change the indices of it instead of directly change the string array
    // this part was helped a lot by TAs during section when converting my old insertion sort into the new sort
    List sortedIndex = newList();
    append(sortedIndex, 0); // add the first element into the list to fullfilled the requirement for the loop
	
    for(int i = 1; i < lines; i++){
        moveFront(sortedIndex); // start at the front of the list
        int j = 0; // keep track of current index
        
        while (j < length(sortedIndex)) { // move through the List until it finds the correct position for the current index
            if (strcmp(array[get(sortedIndex)], array[i]) > 0) {
                insertBefore(sortedIndex, i); // insert the current index into the correct position
                break;
            }
            
            moveNext(sortedIndex); // go to next node
            j++;
        }
	
        if(j == length(sortedIndex)) { // if the current index is not inserted (should be at the end), use append
            append(sortedIndex, i);
        }
    }
    
    for (moveFront(sortedIndex); index(sortedIndex) != -1; moveNext(sortedIndex)) { //print array to out file
        fprintf(out, "%s", array[get(sortedIndex)]);
    }

    // free the memory and close files
    freeList(&sortedIndex);
    for (int i = 0; i < lines; i++) {
        free(array[i]);
    }
    free(array);
    fclose(in);
    fclose(out);
    return 1;
}
