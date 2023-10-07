/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Winter CSE101 PA3
* GraphTest.c
* Testing for Graph.c
*********************************************************************************/
// Very similar concept to my GraphTest in pa2, where I used in1.txt as the input and out1.txt as guided output
#include "Graph.h"

int main(){
    // putting in in1.txt to make the graph
    Graph G = newGraph(9);
    List S = newList();
    for (int i = 1; i <= 9; i++) {
        append(S, i);
    }
    
    addArc(G, 1, 2);
    addArc(G, 2, 1);
    addArc(G, 2, 3);
    addArc(G, 3, 5);
    addArc(G, 5, 4);
    addArc(G, 4, 3);
    addArc(G, 5, 6);
    addArc(G, 6, 8);
    addArc(G, 7, 6);
    addArc(G, 8, 7);
    addArc(G, 8, 9);
    addArc(G, 1, 3);
    addArc(G, 1, 4);
    addArc(G, 1, 5);
    addArc(G, 1, 6);
    addArc(G, 1, 7);
    addArc(G, 1, 8);
    addArc(G, 1, 9);
    addArc(G, 2, 4);
    addArc(G, 2, 5);
    addArc(G, 2, 6);
    addArc(G, 2, 7);
    addArc(G, 2, 8);
    addArc(G, 2, 9);
    addArc(G, 3, 6);
    addArc(G, 3, 7);
    addArc(G, 3, 8);
    addArc(G, 3, 9);
    addArc(G, 4, 6);
    addArc(G, 4, 7);
    addArc(G, 4, 8);
    addArc(G, 4, 9);
    addArc(G, 5, 7);
    addArc(G, 5, 8);
    addArc(G, 5, 9);
    addArc(G, 6, 9);
    addArc(G, 7, 9);
    addArc(G, 9, 9);
    printGraph(stdout, G); // this should give the same print graph like the example out1.txt file
    DFS(G, S);
    
    printf("Get Discover for 1: %d\n", getDiscover(G, 1)); // should be 1 (first to discover)
    printf("Get Finish for 1: %d\n", getFinish(G, 1)); // should be 18 (2x9 as the last element)
    freeGraph(&G);
    freeList(&S);
    return 1;
}
