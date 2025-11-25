#include <stdio.h>
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
    add_edge(g, 0, 3, 2.0); // direct but heavier edge

    printf("The map of Middle-Graph is revealed:\n");
    print_ant_graph(g);

    // Colony parameters
    AntColony colony = {
        .num_ants = 3,
        .pheromone_weight = 1.0,
        .distance_weight = 2.0,
        .evaporation_rate = 0.1,
        .deposit_amount = 1.0
    };

    // Open a log file for this test run
    FILE* logfile = fopen("test_output.txt", "w");
    if (!logfile) {
        perror("Failed to open log file");
        return 1;
    }

    printf("\nRunning the Ant Colony Optimization saga...\n");
    run_aco(g, &colony, 0, 3, 3, logfile);

    // --- Assertions ---
    double shortcut_pheromone = g->edges[0][3].pheromone;
    double longpath_pheromone = g->edges[0][1].pheromone;

    assert(shortcut_pheromone > 1.0);
    assert(longpath_pheromone > 1.0);
    assert(shortcut_pheromone < 10.0);
    assert(longpath_pheromone < 10.0);

    printf("The quest is victorious: pheromone trails shine brighter than before.\n");

    free_ant_graph(g);
    fclose(logfile);   // close the file
    return 0;
}
