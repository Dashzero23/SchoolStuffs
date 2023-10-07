/********************************************************************************* 
* Nguyen Vu, npvu
* 2023 Winter CSE101 PA2
* Graph.c
* Implementation of GraphADT
*********************************************************************************/
// constructor and access functions were greatly inspired by functions in List.c from pa1
// addEdge and addArc were explained and helped a lot by TAs during section
// BFS and getPath functions were greatly inspired from Graph Algorithm example file
// I realized color was set as int instead of char so it conflicted with some functions
// changing it to char gave me seg fault, so I asked the TA and they said I should define color as number and keep using int
// TAs helped with addArc again since it does not do a good job vs large number of vertices
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

typedef struct GraphObj* Graph;

typedef struct GraphObj {
    int order;  // number of vertices
    int size;   // number of edges
    int source; // label of the vertex that was most recently used
    int* color; // may be white, gray, or black
    int* distance; // the distance from source s to vertex v
    int* parent; // parent of v in the BFS tree
    List* adjList; // array of adjacency lists
} GraphObj;

Graph newGraph(int n) {
    if (n <= 0) { // check for invalid input
        fprintf(stderr, "Function newGraph: n is less than or equal to 0.\n");
        exit(EXIT_FAILURE);
    }

    Graph G = malloc(sizeof(GraphObj));

    // use n + 1 for calloc size since it's recommended in the pa2.pdf
    G->order = n;
    G->size = 0;
    G->source = NIL;
    G->color = calloc(n + 1, sizeof(int));
    G->distance = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->adjList = calloc(n + 1, sizeof(List));

    for (int i = 1; i < n + 1; i++) { // initialize array for each vertex v, use n + 1 because memory allocated for n + 1 element
        G->color[i] = 0;
        G->parent[i] = NIL;
        G->distance[i] = INF;
        G->adjList[i] = newList();
    }

    return G;
}

void freeGraph(Graph* pG) {
    if (pG != NULL && *pG != NULL) { // check for invalid input
        Graph G = *pG; // make it G so it's easier to type than *pG

        for (int i = 1; i <= getOrder(G); i++) { // free all heap memory of G, and set it to NULL
            freeList(&G->adjList[i]);
        }

        free(G->color);
        free(G->parent);
        free(G->distance);
        free(G->adjList);
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

int getSource(Graph G) {
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function getSource: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    return G->source;
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

int getDist(Graph G, int u) {
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function getDist: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G)) { // precondition check
        fprintf(stderr, "Function getDist: vertex %d is out of range.\n", u);
        exit(EXIT_FAILURE);
    }

    return G->distance[u];
}

void getPath(List L, Graph G, int u) { // from my understanding, the way this function work is I traceback to the source vertex from the vertex I'm at, 
// then add the path into list L
// in this case, I will use a stack so I can traceback to the source vertex, then print it out to list L in the reverse way
// TA said I can also use recursion, but I don't like recursion
// I changed my mind, I looked at the Graph Algorithms example file and find function Print Path, which can be greatly inspired to make this function a lot simplier
    if (L == NULL) { // check for invalid input
        fprintf(stderr, "Function getPath: List is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function getPath: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (getSource(G) == NIL) { // precondition check
        fprintf(stderr, "Function getPath: BFS was not done.\n");
        exit(EXIT_FAILURE);
    }
    
    if (u < 1 || u > getOrder(G)) { // precondition check
        fprintf(stderr, "Function getPath: vertex %d is out of range.\n", u);
        exit(EXIT_FAILURE);
    }

    if (u == G->source) {
        append(L, u); // instead of using print statement to print the path, add it to list L
    }

    else if (getParent(G, u) == NIL) {
        append(L, NIL); //  appends to L the value NIL if no such path exists
    }
    
    else {
        getPath(L, G, G->parent[u]); // recursive call to getPath
        append(L, u);
    }
}

void makeNull(Graph G) {
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function makeNull: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= getOrder(G); i++) { // clear all edges,  i <= getOrder(G) also means i <= n
        clear(G->adjList[i]);
    }

    G->size = 0;
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

void BFS(Graph G, int s) { // BFS based on the pseudo code in GraphAlgorithms example file
    if (G == NULL) { // check for invalid input
        fprintf(stderr, "Function BFS: Graph is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (s < 1 || s > getOrder(G)) { // precondition check
        fprintf(stderr, "Function BFS: vertex %d is out of range.\n", s);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= getOrder(G); i++) { // reset G,  i <= getOrder(G) means 1 to getOrder(G) inclusively
        G->color[i] = 0; // reset color to "white"
        G->distance[i] = INF;
        G->parent[i] = NIL;
    }

    G->source = s;
    G->color[s] = 1; // discover the source s
    G->distance[s] = 0;
    G->parent[s] = NIL;

    List Q = newList(); // construct a new empty queue
    append(Q, s); // Enqueue(Q, s)

    while (length(Q) > 0) { // while Q is not empty
        int x = front(Q); // x = Dequeue(Q), or first element (last inserted element) of queue Q
        deleteFront(Q); // delete the first element, since Dequeue remove the element

        List adjListX = G->adjList[x];
        //printf("Length %d\n", length(adjListX));
        
        if (length(adjListX) == 0) { // to prevent exitting the program when there is no adjacent vertex
            G->distance[x] = INF;
            return;
        }
        
        for (moveFront(adjListX); index(G->adjList[x]) != -1; moveNext(adjListX)) {
            int y = get(adjListX); // for y in adj[x]

            if (G->color[y] == 0) { // y is undiscovered
                G->color[y] = 1; // discover y
                G->distance[y] = G->distance[x] + 1; // d[y] = d[x]+1
                G->parent[y] = x; // p[y] = x
                append(Q, y); // Enqueue(Q,y)
            }
        }

        G->color[x] = 2; // finish x
    }

    // free the memory
    freeList(&Q);
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

