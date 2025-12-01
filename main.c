// main.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "ant_graph.h"
#include "aco.h"

#define GRAPH_SIZE 50
#define NUM_ANTS 20

int main() {
    printf("ACO program starting...\n");

    FILE* logfile = fopen("aco_output.txt", "w");
    if (!logfile) {
        perror("Failed to open log file");
        return 1;
    }

    // create graph
    AntGraph* g = create_ant_graph(GRAPH_SIZE);

    // add chain edges
    for (int i = 0; i < GRAPH_SIZE - 1; i++) {
        add_edge(g, i, i + 1, 1.1);
    }

    // add shortcuts
    add_edge(g, 0, 20, 2.0);
    add_edge(g, 5, 20, 1.0);
    add_edge(g, 3, 20, 2.0);
    add_edge(g, 8, 20, 1.0);

    // seed pheromones on shortcuts
    g->edges[0][10].pheromone = g->edges[10][0].pheromone = 20.0;
    g->edges[5][19].pheromone = g->edges[19][5].pheromone = 20.0;
    g->edges[3][15].pheromone = g->edges[15][3].pheromone = 20.0;
    g->edges[8][14].pheromone = g->edges[14][8].pheromone = 20.0;

    // create ant colony
    AntColony colony;
    colony.num_ants = NUM_ANTS;
    colony.alpha = 5.0;
    colony.beta = 4.0;
    colony.evaporation_rate = 0.5; // p
    colony.deposit_amount = 10.0; // Q

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
    colony.use_global_best_update = 1;

    // run ACO from start (0) to end (GRAPH_SIZE-1) with 50 iterations
    run_aco(g, &colony, 0, GRAPH_SIZE - 1, 50, logfile);

    free(colony.global_best_path);
    free_ant_graph(g);
    fclose(logfile);

    printf("ACO finished successfully.\n");
    return 0;
}
