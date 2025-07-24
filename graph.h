/*
GROUP NUMBER : 45
    LASTNAME1, FIRSTNAME1: TRINIDAD, NATHAN         SECTION1: S13
    LASTNAME2, FIRSTNAME2: DIMAUNAHAN, CHELSEA JEI  SECTION2: S14
    LASTNAME3, FIRSTNAME3: SALAMIDA, GABRIEL        SECTION3: S13
*/

#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LABEL_LEN (9)
#define MAX_FILE_NAME_LEN (24)

typedef char strName[MAX_LABEL_LEN];  // type for vertex names
typedef char strFile[MAX_FILE_NAME_LEN];

typedef struct _Node {
    strName vertexName;
    struct _Node* edge;
} Node;

typedef struct _List {
    Node* head;
    Node* tail;
} List;

typedef struct _Graph {
    int numVertices;      // number of vertices in the graph
    List* adjList;       // array of Lists
} Graph;

int ReadInputFile(strFile, Graph**);
Node* createNode(strName);
void initList(List*);
void appendNode(List* , strName);
void freeList(List*) ;
Graph* createGraph(int);
void addEdge(Graph*, strName, strName);
Graph* freeGraph(Graph*);
#endif