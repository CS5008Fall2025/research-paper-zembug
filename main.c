#include <stdio.h>
#include <limits.h>
#include "ant_graph.h"
#include "aco.h"

#define GRAPH_SIZE 50 // change graph size here
#define NUM_ANTS 100 // change number of ants here


int main() {
    printf("ACO program starting...\n");

    FILE* logfile = fopen("aco_output.txt", "w"); // overwrite each run
    if (!logfile) {
        perror("Failed to open log file");
        return 1;
    }

    // create a graph with GRAPH_SIZE nodes
    AntGraph* g = create_ant_graph(GRAPH_SIZE);

    // add chain edges (0 → 1 → 2 → … → GRAPH_SIZE-1)
    for (int i = 0; i < GRAPH_SIZE - 1; i++) {
        add_edge(g, i, i+1, 1.1);
    }

    // balanced shortcuts
    add_edge(g, 0, 20, 2.0);
    add_edge(g, 5, 20, 1.0);
    add_edge(g, 3, 20, 2.0);
    add_edge(g, 8, 20, 1.0);

    // seed pheromone on shortcuts
    g->edges[0][10].pheromone = g->edges[10][0].pheromone = 20.0;
    g->edges[5][19].pheromone = g->edges[19][5].pheromone = 20.0;
    g->edges[3][15].pheromone = g->edges[15][3].pheromone = 20.0;
    g->edges[8][14].pheromone = g->edges[14][8].pheromone = 20.0;

    // create an ant colony
    AntColony colony;
    colony.num_ants = NUM_ANTS;   // match colony size to graph size
    colony.evaporation_rate = 0.1;
    colony.pheromone_weight = 2.0;
    colony.distance_weight = 0.5;
    colony.deposit_amount = 1.0;

    colony.global_best_length = INT_MAX;

    // run ACO from start node 0 to end node GRAPH_SIZE-1
    run_aco(g, &colony, 0, GRAPH_SIZE - 1, GRAPH_SIZE, logfile);

    free_ant_graph(g);
    fclose(logfile);

    return 0;
}
