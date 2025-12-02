#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "ant_graph.h"
#include "aco.h"

int main() {
    printf("The Fellowship of the Ants begins their quest...\n");

    // Create a simple graph with 4 nodes
    AntGraph* g = create_ant_graph(4);

    // Add edges: both a shortcut and a longer path
    add_edge(g, 0, 1, 1.0);
    add_edge(g, 1, 2, 1.0);
    add_edge(g, 2, 3, 1.0);
    add_edge(g, 0, 3, 2.0);

    printf("The map of Middle-Graph is revealed:\n");
    print_ant_graph(g);

    // Colony parameters
    AntColony colony = {
        .num_ants = 3,
        .alpha = 1.0,
        .beta = 2.0,
        .evaporation_rate = 0.1,
        .deposit_amount = 1.0
    };

    // Open a log file for this test run
    FILE* logfile = fopen("test_output.txt", "w");
    if (!logfile) {
        perror("Failed to open log file");
        return 1;
    }

    // Initialize global best tracking
    colony.global_best_length = INT_MAX;
    colony.global_best_capacity = g->num_nodes;
    colony.global_best_path = malloc(g->num_nodes * sizeof(int));
    if (!colony.global_best_path) {
        perror("Failed to allocate global best path");
        free_ant_graph(g);
        fclose(logfile);
        return 1;
    }

    // Other flags
    colony.prevent_backtracking = 1;
    colony.max_steps = g->num_nodes;
    colony.use_global_best_update = 0;

    printf("\nRunning the Ant Colony Optimization saga...\n");
    run_aco(g, &colony, 0, 3, 3, logfile);

    // Assertions to verify pheromone levels
    double shortcut_pheromone = g->edges[0][3].pheromone;
    double longpath_pheromone = g->edges[0][1].pheromone;

    assert(shortcut_pheromone > 1.0);
    assert(longpath_pheromone > 1.0);
    assert(shortcut_pheromone < 10.0);
    assert(longpath_pheromone < 10.0);

    printf("The quest is victorious: pheromone trails shine brighter than before.\n");

    free(colony.global_best_path);
    free_ant_graph(g);
    fclose(logfile);   // close the file
    return 0;
}
