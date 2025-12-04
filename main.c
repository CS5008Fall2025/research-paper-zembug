// main.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "ant_graph.h"
#include "aco.h"

#define GRAPH_SIZE 1000 // number of nodes in the graph
#define NUM_ANTS 100    // number of ants in the colony

int main() {
    // Announce program start
    printf("ACO program starting...\n");
    // Open a logfile to record results
    FILE* logfile = fopen("aco_output.txt", "w");
    if (!logfile) {
        perror("Failed to open log file"); // print error if file can't be opened
        return 1; // exit with error code
    }
    // Create a graph with GRAPH_SIZE nodes
    AntGraph* g = create_ant_graph(GRAPH_SIZE);
    // Add chain edges (sequential nodes connected in a line)
    // Each edge has weight 1.1 and initial pheromone set to 1.0
    for (int i = 0; i < GRAPH_SIZE - 1; i++) {
        add_edge(g, i, i + 1, 1.1);
        g->edges[i][i+1].pheromone = g->edges[i+1][i].pheromone = 1.0;
    }

    // Add shortcut edges (longer jumps across the graph)
    // These edges have higher weights but will start with stronger pheromone
    add_edge(g, 0, 10, 2.5);
    add_edge(g, 5, 15, 2.0);
    add_edge(g, 10, 25, 3.0);
    add_edge(g, 20, 35, 2.8);
    add_edge(g, 30, 45, 3.0);

    // Initialize pheromone levels on some shortcut edges to be very high
    g->edges[0][10].pheromone  = g->edges[10][0].pheromone  = 50.0;
    g->edges[5][15].pheromone  = g->edges[15][5].pheromone  = 50.0;
    g->edges[10][25].pheromone = g->edges[25][10].pheromone = 50.0;

    // Create and configure the ant colony
    AntColony colony;
    colony.num_ants = NUM_ANTS;
    colony.alpha = 1.0; // pheromone influence (higher = stronger bias toward pheromone trails)
    colony.beta = 3.0; // heuristic influence (higher = stronger bias toward shorter edges)
    colony.evaporation_rate = 0.5; // pheromone evaporation rate (slower evaporation preserves trails longer)
    colony.deposit_amount = 10.0; // pheromone deposited per ant per path

    // Initialize global best path tracking
    colony.global_best_length = INT_MAX; // no best path yet
    colony.global_best_capacity = GRAPH_SIZE; // maximum possible path length
    colony.global_best_path = malloc(GRAPH_SIZE * sizeof(int));
    if (!colony.global_best_path) {
        perror("Failed to allocate global best path");
        free_ant_graph(g);
        fclose(logfile);
        return 1; // exit if memory allocation fails
    }

    // Additional colony settings
    colony.prevent_backtracking = 1; // ants cannot immediately return to the previous node
    colony.max_steps = GRAPH_SIZE; // maximum steps allowed in a path
    colony.use_global_best_update = 0; // iteration-best ants deposit pheromone (not just global best)

    // Measure runtime of the ACO run
    clock_t start = clock(); // start timing
    run_aco(g, &colony, 0, GRAPH_SIZE - 1, 50, logfile); // run ACO from node 0 to last node 
    clock_t end = clock(); // end timing
    double runtime_sec = (double)(end - start) / CLOCKS_PER_SEC; // calculate elapsed time
    fprintf(logfile, "Total runtime: %.3f seconds\n", runtime_sec); // log runtime
    printf("Total runtime: %.3f seconds\n", runtime_sec);

    // Estimate memory usage
    size_t edge_mem = GRAPH_SIZE * GRAPH_SIZE * sizeof(Edge); // adjacency matrix
    size_t path_mem = GRAPH_SIZE * sizeof(int); // global best path
    size_t total_mem = edge_mem + path_mem; // total estimated memory
    fprintf(logfile, "Estimated memory: %.2f MB\n", total_mem / (1024.0 * 1024.0)); // convert to MB
    printf("Estimated memory: %.2f MB\n", total_mem / (1024.0 * 1024.0));

    // Clean up memory and close files
    free(colony.global_best_path);
    free_ant_graph(g);
    fclose(logfile);

    // Announce program completion
    printf("ACO finished successfully.\n");
    return 0; // exit successfully
}

