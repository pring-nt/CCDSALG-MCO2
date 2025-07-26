#include "graph.c"
/*
    Purpose: Prints the adjacency‐list representation of the graph to stdout.
    Returns: void
    @param  : g — pointer to the Graph to print
    Pre-condition:
             - g must not be NULL.
    Post-condition:
             - For each vertex i, prints its label followed by all neighbors.
             - Format: VertexName -> Neighbor1 -> Neighbor2 -> ... -> NULL
*/
void printGraph(const Graph* g) {
    for (int i = 0; i < g->numVertices; i++) {
        Node* cur = g->adjList[i].head;
        if (!cur) {
            // empty list (shouldn't happen if every vertex has itself as head)
            printf("List[%d]: (empty)\n", i);
            continue;
        }
        // print the source vertex
        printf("%s", cur->vertexName);
        // print all neighbors
        cur = cur->edge;
        while (cur) {
            printf(" -> %s", cur->vertexName);
            cur = cur->edge;
        }
        printf(" -> NULL\n");
    }
}

int main() {
    Graph* g = NULL;
    strFile fileName;
    printf("Input filename: ");
    scanf("%s", fileName);

    int retVal = ReadInputFile(fileName, &g);

    if (retVal == 0) {
        printf("File Z.TXT not found.\n");
        exit(1);
    }

    strName vertex;
    printf("Input start vertex for traversal: ");
    scanf("%s", vertex);

    strName dfs[g->numVertices];
    strName bfs[g->numVertices];

    
    int dfsIndex = DFS(g, vertex, dfs);
    int bfsIndex = BFS(g, vertex, bfs);

    int i;
    printf("BFS: \n");
    for(i = 0; i < bfsIndex; i++) {
        printf("%s ", bfs[i]);
    }
    printf("\n");

    printf("DFS: \n");
    for(i = 0; i < dfsIndex; i++) {
        printf("%s ", dfs[i]);
    }
    printf("\n");

    printGraph(g);
    g = freeGraph(g);
    return 0;
}