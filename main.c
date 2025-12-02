// main.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "ant_graph.h"
#include "aco.h"

#define GRAPH_SIZE 50
#define NUM_ANTS 100

int main() {
    printf("ACO program starting...\n");

    FILE* logfile = fopen("aco_output.txt", "w");
    if (!logfile) {
        perror("Failed to open log file");
        return 1;
    }

    // create graph
    AntGraph* g = create_ant_graph(GRAPH_SIZE);

    // add chain edges with low pheromone
    for (int i = 0; i < GRAPH_SIZE - 1; i++) {
        add_edge(g, i, i + 1, 1.1);
        g->edges[i][i+1].pheromone = g->edges[i+1][i].pheromone = 1.0;
    }

    // add shortcut edges with higher pheromone
    add_edge(g, 0, 10, 2.5);
    add_edge(g, 5, 15, 2.0);
    add_edge(g, 10, 25, 3.0);
    add_edge(g, 20, 35, 2.8);
    add_edge(g, 30, 45, 3.0);

    g->edges[0][10].pheromone = g->edges[10][0].pheromone = 50.0;
    g->edges[5][15].pheromone = g->edges[15][5].pheromone = 50.0;
    g->edges[10][25].pheromone = g->edges[25][10].pheromone = 50.0;

    // create ant colony
    AntColony colony;
    colony.num_ants = NUM_ANTS;
    colony.alpha = 1.5;            // pheromone influence
    colony.beta = 3.0;             // heuristic influence (favor shorter edges)
    colony.evaporation_rate = 0.4; // slower evaporation to preserve useful trails
    colony.deposit_amount = 10.0;  // pheromone deposit per ant

    colony.global_best_length = INT_MAX;
    colony.global_best_capacity = GRAPH_SIZE;
    colony.global_best_path = malloc(GRAPH_SIZE * sizeof(int));
    if (!colony.global_best_path) {
        perror("Failed to allocate global best path");
        free_ant_graph(g);
        fclose(logfile);
        return 1;
    }

    colony.prevent_backtracking = 1;
    colony.max_steps = GRAPH_SIZE;
    colony.use_global_best_update = 0; // allow iteration-best ants to deposit pheromone

    // run ACO from start (0) to end (GRAPH_SIZE-1) with 50 iterations
    run_aco(g, &colony, 0, GRAPH_SIZE - 1, 50, logfile);

    free(colony.global_best_path);
    free_ant_graph(g);
    fclose(logfile);

    printf("ACO finished successfully.\n");
    return 0;
}
