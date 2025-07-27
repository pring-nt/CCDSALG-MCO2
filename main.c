#include "graph.h"

int main() {
    Graph* g = NULL;
    strFile fileName;
    strName vertex;

    /* 1. Read input file */
    printf("Input filename: ");
    scanf("%s", fileName);

    /* 2. Process graph file */
    if (!ReadInputFile(fileName, &g)) {
        printf("File %s not found.\n", fileName);
        exit(1);
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
        exit(1);
    }
    else    // If vertex exists, produce outputs 5 and 6
    {
            ProduceBFSFile(g, fileName, vertex);  // Output #5: BFS traversal
            ProduceDFSFile(g, fileName, vertex);  // Output #6: DFS traversal
    }

    /* 6. Cleanup */
    g = freeGraph(g);

    return 0;
}
