/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Winter CSE101 PA3
* Graph.c
* Implementation of Graph ADT
*********************************************************************************/
// constructor and access functions were greatly inspired by functions in List.c from pa1
// all duplicate functions from PA2 were copied/pasted
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

typedef struct GraphObj* Graph;

typedef struct GraphObj {
    int order;  // number of vertices
    int size;   // number of edges
    int source; // label of the vertex that was most recently used
    int* color; // may be white, gray, or black
    int* parent; // parent of v in the BFS tree
    List* adjList; // array of adjacency lists
    int* discover;
    int* finish;
} GraphObj;

Graph newGraph(int n) {
    Graph G = malloc(sizeof(GraphObj));

    // use n + 1 as size since it's recommended in the pa2.pdf
    G->order = n;
    G->size = 0;
    G->source = NIL;
    G->color = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->adjList = calloc(n + 1, sizeof(List));
    G->discover = calloc(n + 1, sizeof(int));
    G->finish = calloc(n + 1, sizeof(int));

    for (int i = 1; i <= n; i++) { // initialize array for each vertex v
        G->parent[i] = NIL;
        G->adjList[i] = newList();
        G->discover[i] = UNDEF;
        G->finish[i] = UNDEF;
    }

    return G;
}

void freeGraph(Graph* pG) { // TA pointed out what might be causing mem leak
    if (pG != NULL && *pG != NULL) { // check for invalid input
        //Graph G = *pG; // make it G so it's easier to type than *pG

        for (int i = 1; i <= getOrder(*pG); i++) { // free all heap memory of G, and set it to NULL
            freeList(&(*pG)->adjList[i]);
        }

        free((*pG)->parent);
        free((*pG)->color);
        free((*pG)->adjList);
        free((*pG)->discover);
        free((*pG)->finish);
        free(*pG);
        *pG = NULL;
    }
}

int getOrder(Graph G) {
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function getOrder: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    return G->order;
}

int getSize(Graph G) {
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function getSize: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    return G->size;
}

int getParent(Graph G, int u) {
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function getParent: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G)) { // precondition check
        fprintf(stderr, "Function getParent: vertex %d is out of range.\n", u);
        exit(EXIT_FAILURE);
    }

    return G->parent[u];
}

int getDiscover(Graph G, int u){
    if(G == NULL){ // check for invalid input
        fprintf(stderr, "Function getDiscover: Graph is NULL\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G)) { // precondition check
        fprintf(stderr, "Function getDiscover: vertex %d is out of range.\n", u);
        exit(EXIT_FAILURE);
    }

    return G->discover[u];
}

int getFinish(Graph G, int u) {
    if(G == NULL){ // check for invalid input
        fprintf(stderr, "Function getDiscover: Graph is NULL\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G)) { // precondition check
        fprintf(stderr, "Function getDiscover: vertex %d is out of range.\n", u);
        exit(EXIT_FAILURE);
    }

    return G->finish[u];
}

void addEdge(Graph G, int u, int v) {
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function addEdge: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G)) { // precondition check
        fprintf(stderr, "Function addEdge: vertex %d is out of range.\n", u);
        exit(EXIT_FAILURE);
    }

    if (v < 1 || v > getOrder(G)) { // precondition check
        fprintf(stderr, "Function addEdge: vertex %d is out of range.\n", v);
        exit(EXIT_FAILURE);
    }

    addArc(G, u, v); // u is added to the adjacency List of v
    addArc(G, v, u); // v is added to the adjacency List of u
    G->size--; // since addArc +1 to size everytime it's called, it would be +2 here if I don't -1 to the size
}

void addArc(Graph G, int u, int v) { // improve addArc, use insertion sort to deal with large number of vertices
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function addArc: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G)) { // precondition check
        fprintf(stderr, "Function addArc: vertex %d is out of range.\n", u);
        exit(EXIT_FAILURE);
    }

    if (v < 1 || v > getOrder(G)) { // precondition check
        fprintf(stderr, "Function addArc: vertex %d is out of range.\n", v);
        exit(EXIT_FAILURE);
    }

    List L = G->adjList[u]; // get the adjacency list of vertex u
    
    if (length(L) == 0) { // if the list is empty, append vertex v
        append(L, v);
        G->size++; // increases size by 1
    }
    
    else { // if the list is not empty
        moveFront(L); // start at the beginning of the list
        
        while (index(L) >= 0 && v > get(L)) { // find the correct position to insert v
            moveNext(L);
        }
        if (index(L) == -1) { // if the end of the list is reached, append v
            append(L, v);
            G->size++; // increases number of edge by 1
        }
        
        else if (get(L) != v) { // if v is not already in the list, insert it
            insertBefore(L, v);
            G->size++; // increases size by 1
        }
    }
}

void Visit(Graph G, List S, int x, int* time);

void DFS(Graph G, List S) { // DFS based on the pseudo code in GraphAlgorithms example file
// have no clue why the length of adjList in Visit keeps getting to 0 and break the code
// helped by TA by fixing List issue as well as debugged for ModelGraphTest
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function DFS: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (S == NULL) { // check for invalid input
        fprintf(stderr, "Function DFS: List is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (length(S) != getOrder(G)) { // precondition check
        fprintf(stderr, "Function DFS: List length does not equal to Graph Order.\n");
        exit(EXIT_FAILURE);
    }

    //List CopyS = copyList(S);
    
    for (int x = 1; x <= getOrder(G); x++) { // for all x in V(G), since we are working with vertices in the graph, it makes more sense to use Graph functions
        G->color[x] = 0; // color[x] = white
        G->parent[x] = NIL; // p[x] = nil
        G->discover[x] = UNDEF;
        G->finish[x] = UNDEF;
    }

    int time = 0; // time = 0
    //printf("DFS: %d\n", length(S));
    
    for (moveFront(S); index(S) != -1; moveNext(S)) { // for all x in V(G)  // main loop of DFS, since we are working with cursor data in the list, it makes more sense to use List functions
        int x = get(S); // set the cursor data to x

        if (G->color[x] == 0) { // if color[x] == white 
            Visit(G, S, x, &time); // Visit(x), also passes in Graph G, List S and time
        }
    }
    
    for (int i = 1; i <= getOrder(G); i++) {
        deleteBack(S);
    }
    //freeList(&CopyS);
}

void Visit(Graph G, List S, int x, int* time) { // was done with the help from TAs explaining the pseudo code and what to do with the stack (list S)
// debugged by TA and fixed some minor issues
    G->discover[x] = ++(*time); // d[x] = (++time) // discover x
    G->color[x] = 1; // color[x] = gray

    List adj = G->adjList[x]; // set the adjacency list to adj
    //printf("Visit: %d\n", length(adj));
    //printf("Visit X: %d\n", x);
    if (length(adj) == 0) { // check for special case
        G->color[x] = 2;
        G->finish[x] = ++(*time);
        prepend(S, x);
        return;
    }
    
    //List CopyS = copyList(*S); // make a copy of S, again
        
    for (moveFront(adj); index(adj) != -1; moveNext(adj)) { // for all y in adj[x]
        int y = get(adj); // set cursor data to y

        if (G->color[y] == 0) { // if color[y] == white
            G->parent[y] = x; // p[y] = x
            Visit(G, S, y, time); // Visit(y)
        }
    }

    G->color[x] = 2; // color[x] = black 
    G->finish[x] = ++(*time); // f[x] = (++time) // finish x
    prepend(S, x); // // when finished, push to Stack, or list S in this case
    //freeList(&CopyS);
}

Graph transpose(Graph G) {
    // printf("%d\n", getOrder(G));
    Graph Transpose = newGraph(getOrder(G)); // make transpose graph

    for (int i = 1; i <= getOrder(G); i++) {
        List L = G->adjList[i];
        //printf("transpose: %d\n", length(L));
        //List tempL = newList(); // create a temporary list to hold the adjacency list
        
        if (length(L) > 0) { // check for proper list length
            for (moveFront(L); index(L) != -1; moveNext(L)) { // copy all vertices but with reverse edge (or simply reverse the adjacency list of all vertices)
                int vertex = get(L);
                addArc(Transpose, vertex, i);
                //append(tempL, vertex); // add the vertex to the temporary list
            }
        }
        
        //G->adjList[i] = tempL; // update the adjacency list with the temporary list
    }

    return Transpose;
}

Graph copyGraph(Graph G) { // very similar concept to transpose, just not reversing the edge in the copying loop
    Graph Copy = newGraph(getOrder(G)); // make copy graph

    for (int i = 1; i <= getOrder(G); i++) {
        List L = G->adjList[i];

        for (moveFront(L); index(L) != -1; moveNext(L)) { // copy all vertices without reversing the edge
            addArc(Copy, i, get(L));
        }
    }

    return Copy;
}

void printGraph(FILE* out, Graph G) { // improve printGraph function by printing things out 1 by 1 instead of printList, helped with debugging
    if (out == NULL) { // check for invalid input
        fprintf(stderr, "Function printGraph: Output file is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function printGraph: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= getOrder(G); i++) {
        fprintf(out, "%d: ", i); // print out vertice index
        List adj_list = G->adjList[i]; 
        
        if (length(adj_list) > 0) { // check if list is empty
            moveFront(adj_list); // starts from the front
            
            while (index(adj_list) >= 0) { // loop through the list
                int adj_vertex = get(adj_list); // get and print out vertice
                fprintf(out, "%d", adj_vertex);
                moveNext(adj_list);
                
                if (index(adj_list) >= 0) { // if there's still more vertex in the adjacent list, print a space
                    fprintf(out, " ");
                }
            }
        }
        fprintf(out, "\n");
    }
}
