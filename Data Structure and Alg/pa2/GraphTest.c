/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Winter CSE101 PA2
* GraphTest.c
* Testing for Graph.c
*********************************************************************************/
// I will use in1.txt file as the input data to test my functions
// I used out1.txt as a guide to code the output data as well as BFS
#include "Graph.h"

int main(){
    // putting in in1.txt to make the graph
    Graph G = newGraph(15);
    addEdge(G, 1, 7);
    addEdge(G, 2, 3);
    addEdge(G, 2, 6);
    addEdge(G, 3, 7);
    addEdge(G, 4, 10);
    addEdge(G, 5, 9);
    addEdge(G, 5, 10);
    addEdge(G, 6, 7);
    addEdge(G, 6, 11);
    addEdge(G, 7, 12);
    addEdge(G, 8, 13);
    addEdge(G, 9, 13);
    addEdge(G, 9, 14);
    addEdge(G, 10, 15);
    addEdge(G, 14, 15);
    printGraph(stdout, G); // this should give the same print graph like the example out1.txt file

    List L = newList();
    
    // first search
    BFS(G, 1);
    printf("\nThe distance from 1 to 12 is %d\n", getDist(G, 12));
    printf("The shortest path from 1 to 12 is: ");
    getPath(L, G, 12);
    printList(stdout, L);
    printf("\n");
    // the output should be the same as the first search in out1.txt
    
    clear(L);
    
    // second search
    BFS(G, 1);
    printf("\nThe distance from 1 to 15 is %d\n", getDist(G, 15));
    printf("The shortest path from 1 to 15 is: ");
    getPath(L, G, 15);
    printList(stdout, L);
    printf("\n");
    // the output should give -1 for distance (INF in my code) and -2 for path (NIL in my code)
    
    // free the memory
    freeGraph(&G);
    freeList(&L);
    return 0;
}
