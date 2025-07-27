#include "graph.h"

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
            printf("List[%d]: (empty)\n", i);
            continue;
        }
        printf("%s", cur->vertexName);
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
    strName vertex;

    /* 1. Read input file */
    printf("Input filename: ");
    scanf("%s", fileName);

    /* 2. Process graph file */
    if (!ReadInputFile(fileName, &g)) {  // File does not exist
        printf("File %s not found.\n", fileName);
        exit(1);  // Doesn't proceed with the creation of the output files
    }

    /* 3. Generate all required output files, whether the vertex exists or not */
    ProduceSetsFile(g, fileName);     // Output #1: V(G) and E(G)
    ProduceDegreeFile(g, fileName);   // Output #2: Vertex degrees
    ProduceListFile(g, fileName);     // Output #3: Adjacency list
    ProduceMatrixFile(g, fileName);   // Output #4: Adjacency matrix

    /* 4. Get traversal starting vertex */
    printf("Input start vertex for traversal: ");
    scanf("%s", vertex);
    
    /* 5. Check if starting vertex exists */
    if (getVertexIndex(g, vertex) < 0) {    // Does not exist
        printf("Vertex %s not found.\n", vertex);
        g = freeGraph(g);
        exit(1);  // Does not proceed with the creation of outputs 5 & 6
    }
    else    // If vertex exists
    {
            ProduceBFSFile(g, fileName, vertex);  // Output #5: BFS traversal
            ProduceDFSFile(g, fileName, vertex);  // Output #6: DFS traversal
            
            // Print traversals to console
            strName dfs[g->numVertices], bfs[g->numVertices];
            int dfsCount = DFS(g, vertex, dfs);
            int bfsCount = BFS(g, vertex, bfs);
            
            printf("BFS: ");
            for(int i = 0; i < bfsCount; i++) 
            {
                printf("%s ", bfs[i]);
            }
            printf("\nDFS: ");

            for(int i = 0; i < dfsCount; i++)
            {
                printf("%s ", dfs[i]);
            } 
            printf("\n");
    }

    /* 6. Cleanup */
    printGraph(g);  // Print graph to console
    g = freeGraph(g);

    return 0;
}
