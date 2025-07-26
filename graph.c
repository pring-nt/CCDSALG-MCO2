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
               reflect the file's contents.
             - File pointer is closed before returning.
             - On failure, *GDS is left unmodified.
*/
int ReadInputFile(strFile strInputFileName, Graph** GDS) {
    // 1) Open the file
    FILE* fp = fopen(strInputFileName, "r");
    if (fp == NULL) {
        fprintf(stderr, "File %s not found.", strInputFileName);
        exit(1); // File not found, terminate the program 
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
    strName flag = "-1";      // sentinel marking end of one vertex's neighbors
    int i, exitFlag;
    
    // 4) For each vertex, read its line of adjacency info
    for (i = 0; i < numVertex; i++) {
        exitFlag = 0;

        // 4a) First token is the vertex's own label
        fscanf(fp, "%s", token);
        appendNode(&newGraph->adjList[i], token);

        // 4b) Subsequent tokens are neighbors until we hit "-1"
        while (!exitFlag) {
            fscanf(fp, "%s", token);
            if (strcmp(token, flag) != 0) {
                appendNode(&newGraph->adjList[i], token);
            } else {
                exitFlag = 1; // done with this vertex's neighbor list
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
             - A Node is malloc'd, vertexName set, edge = NULL.
             - Program exits on malloc failure.
*/
Node* createNode(strName name) {
    Node* n = malloc(sizeof(Node));
    if (n == NULL) {
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
             - Graph* is malloc'd.
             - adjList array is calloc'd and each List initialized.
             - On failure, allocated memory is freed and NULL is returned.
*/
Graph* createGraph(int numVertices) {
    Graph* g = malloc(sizeof(Graph));
    if (g == NULL) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!\n");
        return NULL;
    }

    g->numVertices = numVertices;

    if(numVertices == 0) {
        g->adjList = NULL;
        return g;
    }

    // one contiguous block of List structs
    g->adjList = calloc(numVertices, sizeof(List));
    if (g->adjList == NULL) {
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
    // scan through each list's head for the 'from' label
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
    // free each list's nodes
    for (int i = 0; i < g->numVertices; i++) {
        freeList(&g->adjList[i]);
    }
    // free the array of lists, then the graph itself
    free(g->adjList);
    free(g);
    return NULL;
}

/**
    Purpose: Maps a vertex name to its index in the adjacency list.
    Returns: Index of the vertex if found; -1 otherwise.
    @param  : g    - pointer to the Graph.
    @param  : name - vertex name to search.
    Pre-condition:
             - g must not be NULL.
             - name must be a valid, null‑terminated string.
    Post-condition:
             - No modifications to g.
*/
int getVertexIndex(const Graph* g, const strName name) {
    for (int i = 0; i < g->numVertices; i++) {
        if (g->adjList[i].head != NULL &&
            strcmp(g->adjList[i].head->vertexName, name) == 0) {
            return i;
        }
    }
    return -1;  // not found
}

/**
    Purpose: Gathers neighbor indices of a vertex, sorts them alphabetically by name, and returns the count.
    Returns: Number of neighbors found.
    @param  : g     - pointer to the Graph.
    @param  : vIndex  - index of the vertex whose neighbors to collect.
    @param  : out   - pre-allocated array to fill with sorted neighbor indices.
    Pre-condition:
             - g must not be NULL.
             - 0 <= vIndex < g->numVertices.
             - out must point to an array of at least g->numVertices ints.
    Post-condition:
             - outIndex[0..count-1] contains neighbor indices sorted ascending order by vertexName.
*/
int getSortedNeighbors(const Graph* g, int vIndex, int outIndex[]) {
    int count = 0;
    // Collect the neighbor indices
    Node* cur = g->adjList[vIndex].head->edge;
    while (cur != NULL) {
        int idx = getVertexIndex(g, cur->vertexName);
        if (idx >= 0) {
            outIndex[count] = idx;
            count++;
        }
        cur = cur->edge;
    }
    // Selection sort on the indices by comparing vertexName
    for (int i = 0; i < count - 1; i++) {
        int min = i;
        for (int j = i + 1; j < count; j++) {
            const char* nameJ = g->adjList[outIndex[j]].head->vertexName;
            const char* nameMin = g->adjList[outIndex[min]].head->vertexName;
            if (strcmp(nameJ, nameMin) < 0) {
                min = j;
            }
        }
        if (min != i) { // Swap
            int temp = outIndex[i];
            outIndex[i] = outIndex[min];
            outIndex[min] = temp;
        }
    }
    return count;
}

/**
    Purpose: Recursive helper for DFS traversal, stores visitation in traversal[] and marks visited
    Returns: void
    @param  : g         - pointer to the Graph
    @param  : vIndex      - current vertex index
    @param  : visited   - int array marking visited vertices (1 = visited, 0 = not)
    @param  : traversal - array of strName to record visitation order
    @param  : travIndex - pointer to current index in traversal array
    Pre-condition:
        - g, visited, traversal, travIndex must not be NULL
        - visited and traversal arrays size >= g->numVertices
    Post-condition:
        - traversal[0..*travIndex-1] holds names in DFS order
*/
void DFSHelper(const Graph* g, int vIndex, int visited[], strName traversal[], int* travIndex) {
    // mark and record this vertex
    visited[vIndex] = 1;
    strcpy(traversal[*travIndex], g->adjList[vIndex].head->vertexName);
    *travIndex += 1;

     // get sorted neighbor indices
    int neighbors[g->numVertices];
    int neighborCount = getSortedNeighbors(g, vIndex, neighbors);

     // visit each neighbor in order
    for (int i = 0; i < neighborCount; i++) {
        int u = neighbors[i];
        if (!visited[u]) {
            DFSHelper(g, u, visited, traversal, travIndex);
        }
    }
}

/**
    Purpose: Performs a depth-first search starting from a given vertex name.
    Returns: Number of vertices traversed
    @param  : g         - pointer to the Graph
    @param  : startName - name of the start vertex
    @param  : traversal - array of strName to fill with DFS visit order
    Pre-condition:
        - g and traversal must not be NULL
        - traversal array size >= g->numVertices
        - startName must correspond to an existing vertex
    Post-condition:
        - traversal[0..n-1] holds the DFS order; n = number of reachable vertices
*/
int DFS(const Graph* g, const strName startName, strName traversal[]) {
     // allocate and initialize to zero visited flags
    int *visited = calloc(g->numVertices, sizeof(*visited));
    if (visited == NULL) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!\n");
        exit(1);
    }
    int travIndex = 0;

    // find the start vertex
    int start = getVertexIndex(g, startName);
    if (start < 0) {
        fprintf(stderr, "Vertex %s not found", startName);
        free(visited);
        exit(1);
    }

    // run recursive DFS
    DFSHelper(g, start, visited, traversal, &travIndex);

    // clean up
    free(visited);
    return travIndex;
}

/**
        Purpose: Performs a breadth-first search starting from a given vertex name and records order in traversal[].
        Returns: Number of vertices traversed
        @param  : g         - pointer to the Graph
        @param  : startName - name of the start vertex
        @param  : traversal - array of strName to fill with BFS visit order
        Pre-condition:
            - g and traversal must not be NULL
            - traversal array size >= g->numVertices
            - startName corresponds to an existing vertex
        Post-condition:
            - traversal[0..n-1] holds the BFS order; n = number of reachable vertices
*/
int BFS(const Graph* g, const strName startName, strName traversal[]) {
     // allocate and zero visited flags
    int *visited = calloc(g->numVertices, sizeof(*visited));
    if (visited == NULL) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!\n");
        exit(1);
    }
    // allocate queue for vertex indices
    int *queue = calloc(g->numVertices, sizeof(*queue));
    if (queue == NULL) {
        fprintf(stderr, "Error: MEMORY ALLOCATION FAILED!\n");
        free(visited);
        exit(1);
    }

    int front = 0, rear = 0;
    int travIndex = 0;

    // find start vertex index
    int start = getVertexIndex(g, startName);
    if (start < 0) {
        fprintf(stderr, "Vertex %s not found", startName);
        exit(1);
    }
    // enqueue and record the start vertex
    visited[start] = 1;
    queue[rear] = start;
    rear++;

    // process queue
    while (front < rear) {
        int v = queue[front];
        front++;
        strcpy(traversal[travIndex], g->adjList[v].head->vertexName);
        travIndex++;
        // get sorted neighbor indices
        int neighbors[g->numVertices];
        int cnt = getSortedNeighbors(g, v, neighbors);
        for (int i = 0; i < cnt; i++) {
            int u = neighbors[i];
            if (!visited[u]) {
                visited[u] = 1;
                queue[rear] = u;
                rear++;
            }
        }
    }
    // clean up
    free(visited);
    free(queue);
    return travIndex;
}
