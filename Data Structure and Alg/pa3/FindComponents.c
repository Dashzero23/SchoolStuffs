/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Winter CSE101 PA2
* FindComponent.c
* Main program for the assignment
*********************************************************************************/
// the input/output file and getting data from input file part was copied/pasted from FindPath.c from PA2
// the main loop was explained and helped by TAs during sections
// the
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
    int source, destination;

    while (fscanf(in, "%d %d\n", &source, &destination) == 2) { // if != 2, then it can't read 2 intergers, which means NULL or end of file, hence the loop stops
    	if (source == 0 || destination == 0) { // check for end of file
    	    break;
    	}
    	
    	else {
            addArc(G, source, destination); // add all edges from the in file
        }
    }

    // Print the graph
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G); // print the graph to out file

    // run DFS on G
    List S = newList();

    for (int i = 1; i <= vertices; i++) {
        append(S, i);
    }
    
    //printf("First DFS\n");
    DFS(G, S);

    // perform DFS on transpose of G
    Graph TransposeG = transpose(G);
    //printf("2nd DFS\n");    
    DFS(TransposeG, S);

    // count the strongly connected components
    int totalSCC = 0;
    // printf("%d\n", length(S));
    
    for (moveFront(S); index(S) >= 0; moveNext(S)) {
        int vertex = get(S);
        
        if (getParent(TransposeG, vertex) == NIL) {
            totalSCC++;
        }
    }

    // print totalSCC to out file
    fprintf(out, "\nG contains %d strongly connected components:\n", totalSCC);

    // create an array of lists that store the SCC
    List *ArraySCC = calloc(totalSCC, sizeof(List));
    
    for (int i = 0; i < totalSCC; i++) {
        ArraySCC[i] = newList();
    }

    int indexSCC = 0;

    for (moveFront(S); index(S) >= 0; moveNext(S)) {
        int cur_vertex = get(S);
   
        if (getParent(TransposeG, cur_vertex) == NIL) {
            indexSCC++;
        }
        
        append(ArraySCC[indexSCC - 1], cur_vertex);
    }

    // print the SCCs in topologically sorted order
    for (int i = totalSCC -1; i >= 0; i--) { 
        fprintf(out, "Component %d: ", totalSCC - i);
        printList(out, ArraySCC[i]);
        fprintf(out, "\n");
 
        // free memory allocated for each list in SCC
        freeList(&ArraySCC[i]);
    }

    // free the memory and close files
    free(ArraySCC); // since I allocate memory for a special list instead of using newList, I don't need to use freeList
    freeGraph(&G);
    freeGraph(&TransposeG);
    freeList(&S);
    fclose(in);
    fclose(out);
}
