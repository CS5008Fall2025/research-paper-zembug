#include <stdio.h>
#include "ant_graph.h"


int main() {
    AntGraph* g = create_ant_graph(4); // create a graph with 4 nodes

    // add some edges
    add_edge(g, 0, 1, 2.5);
    add_edge(g, 1, 2, 1.0);
    add_edge(g, 2, 3, 3.2);

    // print the adjacency matrix
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            printf("%.1f ", g->edges[i][j]);
        }
        printf("\n");
    }

    free_ant_graph(g); // free the graph memory
    return 0;
}