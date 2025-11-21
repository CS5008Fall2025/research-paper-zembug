#include <stdio.h>
#include "ant_graph.h"
#include "aco.h"


int main() {
    /*AntGraph* g = create_ant_graph(4); // create a graph with 4 nodes

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
}*/
    printf("ACO program starting...\n");

        // create a graph with 4 nodes
        AntGraph* g = create_ant_graph(4);

        // add some edges (weights are distances)
        add_edge(g, 0, 1, 2.0);
        add_edge(g, 1, 2, 1.5);
        add_edge(g, 2, 3, 2.5);
        add_edge(g, 0, 3, 5.0);

        // create an ant colony with 3 ants and evaporation rate 0.1
        AntColony colony;
        colony.num_ants = 3;
        colony.evaporation_rate = 0.1;

        // run ACO from node 0 to node 3 for 5 iterations
        run_aco(g, &colony, 0, 3, 5);

        // free memory
        free_ant_graph(g);

        return 0;
    }