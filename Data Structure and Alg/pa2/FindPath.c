/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Winter CSE101 PA2
* FindPath.c
* Main program for the assignment
*********************************************************************************/
// the input/output file part was copied/pasted from Lex.c from PA1
// the main loop was explained and helped by TAs during sections
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"

int main(int argc, char * argv[]) {
    FILE *in, *out;

    // check command line for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // open files for reading and writing
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    if (in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (out == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // read graph from file
    int vertices;
    fscanf(in, "%d\n", &vertices);

    Graph G = newGraph(vertices); // make graph based on read data
    int source, distance;

    while (fscanf(in, "%d %d\n", &source, &distance) == 2) { // if != 2, then it can't read 2 intergers, which means NULL or end of file, hence the loop stops
    	if (source == 0 || distance == 0) { // check for end of file
    	    break;
    	}
    	
    	else {
            addEdge(G, source, distance); // add all edges from the in file
        }
    }

    // Print the graph
    // fprintf(out, "Original graph:\n");
    printGraph(out, G); // print the graph to out file

    // loop through each pair of source and destination vertices from input file
    //int counter = 0;
    
    while (fscanf(in, "%d %d", &source, &distance) == 2) { // same concept as earlier, if != 2, then NULL or end of file
        
        if (source == 0 || distance == 0) { // check for end of file
    	    break;
    	}
    	
    	else {
    	    //counter++;
    	    //printf("Loop %d, %d\n", counter, source);
            List L = newList(); // create a new list to store the path
            int distanceLength; // use to store the distance for output
            
            BFS(G, source); // run BFS starting from the source vertex
        
            if (getDist(G, distance) == INF) { // check if a path exists between the source and destination vertices
                fprintf(out, "\nThe distance from %d to %d is infinity", source, distance);
                fprintf(out, "\nNo %d-%d path exists", source, distance); // if no path exists, print an error message
                fprintf(out, "\n");
            } 
        
            else { // if a path exists, print the path
                getPath(L, G, distance);
                distanceLength = length(L) - 1; // to remove index 0, or NIL
                fprintf(out, "\nThe distance from %d to %d is %d", source, distance, distanceLength);
                fprintf(out, "\nA shortest %d-%d path is: ", source, distance);
                printList(out, L);
                fprintf(out, "\n");
            }

            freeList(&L); // free the list memory
        }
    }

    // free the memory and close files
    freeGraph(&G);
    fclose(in);
    fclose(out);
    return 1;
}
