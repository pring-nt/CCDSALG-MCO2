/*
GROUP NUMBER : 45
    LASTNAME1, FIRSTNAME1: TRINIDAD, NATHAN         SECTION1: S13
    LASTNAME2, FIRSTNAME2: DIMAUNAHAN, CHELSEA JEI  SECTION2: S14
    LASTNAME3, FIRSTNAME3: SALAMIDA, GABRIEL        SECTION3: S13
*/

#include "graph.h"

/*
    Purpose: Creates and initializes a graph structure with a specified number of vertices.
    Returns: Pointer to the created Graph struct.
    @param  : numVertices - number of vertices the graph will contain
    Pre-condition:
             - numVertices must be >= 0 and <= MAX_VERTICES (as defined elsewhere).
    Post-condition:
             - A dynamically allocated Graph* is returned.
             - The adjacency matrix and vertex name array are allocated and initialized to 0.
             - If memory allocation fails, the program exits with an error.
*/
Graph* createGraph(int numVertices) {
    Graph* g = malloc(sizeof(Graph));
    if (g == NULL) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!");
        exit(1);
    }

    g->numVertices = numVertices;

    g->vertexNames = calloc(numVertices, sizeof(strName));
    if (g->vertexNames == NULL) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!");
        exit(1);
    }

    g->adjMatrix = malloc(numVertices * sizeof(int*));
    if (g->adjMatrix == NULL) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!");
        exit(1);
    }

    for (int i = 0; i < numVertices; i++) {
        g->adjMatrix[i] = calloc(numVertices, sizeof(int));
        if (g->adjMatrix[i] == NULL) {
            fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!");
            exit(1);
        }
    }

    return g;
}

/*
    Purpose: Assigns a vertex name to the next available index in the graph, or returns an existing index.
    Returns: Index of the vertex in the graph’s vertexNames array.
    @param  : g         - pointer to the Graph
    @param  : name      - vertex name to assign
    @param  : nextIndex - pointer to the next available index in the vertexNames array
    Pre-condition:
             - g must not be NULL.
             - name must be a valid, null-terminated string of <= 8 characters.
             - *nextIndex must be within bounds (i.e., < g->numVertices).
    Post-condition:
             - If the name exists, its existing index is returned.
             - If it's new, it is assigned to the next index and that index is returned.
             - *nextIndex is incremented.
             - Exits with an error if too many unique vertices are added.
*/
int assignVertex(Graph* g, strName name, int* nextIndex) {
    int idx = getVertexIndex(g, name);
    if (idx != -1) return idx;

    if (*nextIndex >= g->numVertices) {
        fprintf(stderr, "Error: Too many vertices.\n");
        exit(1);
    }

    int assignedIndex = *nextIndex;
    strcpy(g->vertexNames[assignedIndex], name);
    (*nextIndex)++;
    return assignedIndex;
}

/*
    Purpose: Returns the index of a vertex name in the graph’s vertexNames array.
    Returns: Index of the vertex if found; -1 if not found.
    @param  : g    - pointer to the Graph
    @param  : name - name of the vertex to search for
    Pre-condition:
             - g must not be NULL.
             - name must be a valid, null-terminated string.
    Post-condition:
             - Returns the index if the name exists; -1 otherwise.
*/
int getVertexIndex(Graph* g, strName name) {
    for (int i = 0; i < g->numVertices; i++) {
        if (strcmp(g->vertexNames[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

/*
    Purpose: Adds an undirected edge between two vertices in the graph.
    Returns: void
    @param  : g    - pointer to the Graph
    @param  : src  - source vertex name
    @param  : dest - destination vertex name
    Pre-condition:
             - g must not be NULL.
             - Both src and dest must already exist in g->vertexNames.
    Post-condition:
             - If both vertices exist, sets adjMatrix[src][dest] and adjMatrix[dest][src] to 1.
             - If either vertex is missing, an error message is printed.
*/
void addEdge(Graph* g, strName src, strName dest) {
    int i = getVertexIndex(g, src);
    int j = getVertexIndex(g, dest);

    if (i == -1 || j == -1) {
        fprintf(stderr, "One of the vertex IDs is not found.\n");
    } else {
        g->adjMatrix[i][j] = 1;
        g->adjMatrix[j][i] = 1;
    }
}

/*
    Purpose: Frees all memory associated with the graph.
    Returns: void
    @param  : g - pointer to the Graph to free
    Pre-condition:
             - g must not be NULL.
    Post-condition:
             - All dynamically allocated memory inside the graph is freed.
             - The pointer g itself is freed.
*/
void freeGraph(Graph* g) {
    for (int i = 0; i < g->numVertices; i++) {
        free(g->adjMatrix[i]);
    }
    free(g->adjMatrix);
    free(g->vertexNames);
    free(g);
}
