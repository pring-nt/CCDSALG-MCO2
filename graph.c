/*
GROUP NUMBER : 45
    LASTNAME1, FIRSTNAME1: TRINIDAD, NATHAN         SECTION1: S13
    LASTNAME2, FIRSTNAME2: DIMAUNAHAN, CHELSEA JEI  SECTION2: S14
    LASTNAME3, FIRSTNAME3: SALAMIDA, GABRIEL        SECTION3: S13
*/

#include "graph.h"

/*
    Purpose: Reads a graph from a text file and constructs its adjacency lists.
    Returns: 1 if successful; 0 on failure (e.g., file not found or memory error).
    @param  : strInputFileName — name of the input file containing graph data
    @param  : GDS              — address of a Graph* to populate
    Pre-condition:
             - strInputFileName must be a valid, null‑terminated string.
             - GDS must be non‑NULL.
    Post-condition:
             - *GDS points to a newly created Graph whose adjacency lists
               reflect the file’s contents.
             - File pointer is closed before returning.
             - On failure, *GDS is left unmodified.
*/
int ReadInputFile(strFile strInputFileName, Graph** GDS) {
    // 1) Open the file
    FILE* fp = fopen(strInputFileName, "r");
    if (fp == NULL) {
        return 0; // can’t open file
    }

    // 2) Read number of vertices
    int numVertex;
    fscanf(fp, "%d", &numVertex);

    // 3) Create the graph skeleton (lists initialized empty)
    Graph* newGraph = createGraph(numVertex);
    if (!newGraph) {
        fclose(fp);
        return 0;
    }

    strName token;
    strName flag = "-1";      // sentinel marking end of one vertex’s neighbors
    int i, exitFlag;
    
    // 4) For each vertex, read its line of adjacency info
    for (i = 0; i < numVertex; i++) {
        exitFlag = 0;

        // 4a) First token is the vertex’s own label
        fscanf(fp, "%s", token);
        appendNode(&newGraph->adjList[i], token);

        // 4b) Subsequent tokens are neighbors until we hit "-1"
        while (!exitFlag) {
            fscanf(fp, "%s", token);
            if (strcmp(token, flag) != 0) {
                appendNode(&newGraph->adjList[i], token);
            } else {
                exitFlag = 1; // done with this vertex’s neighbor list
            }
        }
    }

    // 5) Hand the new graph back to the caller
    *GDS = newGraph;
    fclose(fp);
    return 1;
}


/*
    Purpose: Allocates and initializes a new adjacency-list node.
    Returns: Pointer to the new Node.
    @param  : name — vertex name to store in the node
    Pre-condition:
             - name must be ≤ MAX_LABEL_LEN‑1 chars, null‑terminated.
    Post-condition:
             - A Node is malloc’d, vertexName set, edge = NULL.
             - Program exits on malloc failure.
*/
Node* createNode(strName name) {
    Node* n = malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!\n");
        exit(1);
    }
    // copy the 8‑char label into the new node
    strcpy(n->vertexName, name);
    n->edge = NULL;
    return n;
}

/*
    Purpose: Initializes an empty List.
    Returns: void
    @param  : l — pointer to the List to initialize
    Pre-condition:
             - l must not be NULL.
    Post-condition:
             - l->head and l->tail are set to NULL.
*/
void initList(List* l) {
    l->head = NULL;
    l->tail = NULL;
}

/*
    Purpose: Appends a new node with given name to the end of the list.
    Returns: void
    @param  : l    — pointer to the List to modify
    @param  : name — vertex name to append
    Pre-condition:
             - l must not be NULL.
    Post-condition:
             - A new Node is created and added at the tail of *l.
             - tail updated to point to the new node.
*/
void appendNode(List* l, strName name) {
    Node* n = createNode(name);
    if (l->head == NULL) {
        // first node in this adjacency list
        l->head = l->tail = n;
    } else {
        // link after the current tail
        l->tail->edge = n;
        l->tail = n;
    }
}

/*
    Purpose: Frees all nodes in a single adjacency list.
    Returns: void
    @param  : l — pointer to the List whose nodes will be freed
    Pre-condition:
             - l must not be NULL.
    Post-condition:
             - All Node structs in the list are freed.
             - The List struct itself remains (but its nodes are gone).
*/
void freeList(List* l) {
    Node* cur = l->head;
    while (cur) {
        Node* temp = cur;
        cur = cur->edge;
        free(temp);
    }
}

/*
    Purpose: Allocates and initializes a Graph with a given number of vertices.
    Returns: Pointer to the newly created Graph; NULL on allocation failure.
    @param  : numVertices — number of vertices in the graph
    Pre-condition:
             - numVertices ≥ 0 and ≤ MAX_VERTICES.
    Post-condition:
             - Graph* is malloc’d.
             - adjList array is calloc’d and each List initialized.
             - On failure, allocated memory is freed and NULL is returned.
*/
Graph* createGraph(int numVertices) {
    Graph* g = malloc(sizeof(Graph));
    if (!g) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!\n");
        return NULL;
    }

    g->numVertices = numVertices;
    // one contiguous block of List structs
    g->adjList = calloc(numVertices, sizeof(List));
    if (!g->adjList) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!\n");
        free(g);
        return NULL;
    }

    // explicitly clear head/tail for each list
    for (int i = 0; i < numVertices; i++) {
        initList(&g->adjList[i]);
    }
    return g;
}

/*
    Purpose: Adds a directed edge from one vertex to another by name.
    Returns: void
    @param  : g    — pointer to the Graph
    @param  : from — name of the source vertex
    @param  : to   — name of the destination vertex
    Pre-condition:
             - g must not be NULL.
             - Vertex named 'from' must already be at head of one List.
    Post-condition:
             - If 'from' is found, a new node with 'to' is appended to its list.
             - On failure, prints an error and exits.
*/
void addEdge(Graph* g, strName from, strName to) {
    int i = 0, found = 0;
    // scan through each list’s head for the 'from' label
    while (i < g->numVertices && !found) {
        if (g->adjList[i].head &&
            strcmp(g->adjList[i].head->vertexName, from) == 0) {
            appendNode(&g->adjList[i], to);
            found = 1;
        } else {
            i++;
        }
    }

    if (!found) {
        fprintf(stderr, "Vertex '%s' not found in graph.\n", from);
        exit(1);
    }
}

/*
    Purpose: Frees an entire Graph, including all adjacency lists and the Graph struct.
    Returns: NULL
    @param  : g — pointer to the Graph to free
    Pre-condition:
             - g must not be NULL.
    Post-condition:
             - All adjacency-list nodes are freed.
             - adjList array and Graph struct are freed.
             - Returns NULL so caller can safely do: g = freeGraph(g);
*/
Graph* freeGraph(Graph* g) {
    // free each list’s nodes
    for (int i = 0; i < g->numVertices; i++) {
        freeList(&g->adjList[i]);
    }
    // free the array of lists, then the graph itself
    free(g->adjList);
    free(g);
    return NULL;
}
