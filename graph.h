/*
GROUP NUMBER : 45
    LASTNAME1, FIRSTNAME1: TRINIDAD, NATHAN         SECTION1: S13
    LASTNAME2, FIRSTNAME2: DIMAUNAHAN, CHELSEA JEI  SECTION2: S14
    LASTNAME3, FIRSTNAME3: SALAMIDA, GABRIEL        SECTION3: S13
*/

#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include <string.h>
#define MAX_LABEL_LEN (9)

typedef char strName[MAX_LABEL_LEN];  // type for vertex names

typedef struct _Graph {
    int numVertices;      // number of vertices in the graph
    strName* vertexNames; // array of vertex names (labels)
    int** adjMatrix;      // adjacency matrix (2D array of ints)
} Graph;

Graph* createGraph(int);
int assignVertex(Graph* g, strName name, int* nextIndex);
int getVertexIndex(Graph*, strName); // Helper function to find a vertex name
void addEdge(Graph*, strName, strName);
void freeGraph(Graph*);
#endif