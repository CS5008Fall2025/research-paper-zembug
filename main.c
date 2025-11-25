#include <stdio.h>
#include <limits.h>
#include "ant_graph.h"
#include "aco.h"

int main() {
    printf("ACO program starting...\n");

    FILE* logfile = fopen("aco_output.txt", "w"); // overwrite each run
    if (!logfile) {
        perror("Failed to open log file");
        return 1;
    }

    // create a graph with n nodes
    AntGraph* g = create_ant_graph(50); // change graph size as needed

    // add chain edges (0 → 1 → 2 → … → n-1)
    for (int i = 0; i < 49; i++) { // update as needed node count - 1
        add_edge(g, i, i+1, 1.1);
    }

    // balanced shortcuts
    add_edge(g, 0, 10, 9.0);   // mid shortcut
    add_edge(g, 5, 19, 14.0);  // late shortcut
    add_edge(g, 3, 15, 12.0);  // cross link
    add_edge(g, 8, 14, 6.0);   // another mid shortcut

    // seed pheromone on shortcuts
    g->edges[0][10].pheromone = g->edges[10][0].pheromone = 20.0;
    g->edges[5][19].pheromone = g->edges[19][5].pheromone = 20.0;
    g->edges[3][15].pheromone = g->edges[15][3].pheromone = 20.0;
    g->edges[8][14].pheromone = g->edges[14][8].pheromone = 20.0;

    // create an ant colony
    AntColony colony;
    colony.num_ants = 50;           // larger colony for richer dynamics
    colony.evaporation_rate = 0.1;  // pheromone evaporation
    colony.pheromone_weight = 2.0;  // influence of pheromone
    colony.distance_weight = 0.5;   // influence of distance
    colony.deposit_amount = 1.0;    // pheromone deposited per ant per step

    // initialize global best tracker
    colony.global_best_length = INT_MAX;

    // run ACO from start node 0, to end node n-1, n iterations
    run_aco(g, &colony, 0, 49, 50, logfile); // change sizes as needed

    // free memory
    free_ant_graph(g);
    fclose(logfile);

    return 0;
}
