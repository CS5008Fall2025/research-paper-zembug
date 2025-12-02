#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ant_graph.h"
#include "aco.h"


/* Run ACO analysis with given parameters and print results */
void run_analysis(int num_nodes, double evap, double pher_w, double dist_w, int iterations, int num_ants) {
    // Create a new graph with the given number of nodes
    AntGraph* g = create_ant_graph(num_nodes);
    // Build a simple line graph: nodes connected in sequence
    for (int i = 0; i < num_nodes-1; i++) {
        add_edge(g, i, i+1, 1.0); // each edge has cost 1.0
    }
    add_edge(g, 0, num_nodes-1, 2.0); // add a shortcut edge from first to last node
    // Initialize the ant colony with parameters
    AntColony colony = {
        .num_ants = num_ants,
        .alpha = pher_w, // pheromone weight
        .beta = dist_w, // distance/heuristic weight
        .evaporation_rate = evap, // evaporation rate
        .deposit_amount = 1.0 // fixed pheromone deposit amount
    };
    // Start timing the run
    clock_t start = clock();
    // Track convergence: when most ants consistently choose the optimal path
    int convergence_iter = -1; // iteration where convergence first happened
    int consecutive = 0; // count of consecutive iterations with high agreement
    // Run the colony for the specified number of iterations
    for (int it = 1; it <= iterations; it++) {
        // Run one iteration and count how many ants chose the optimal path
        int optimal_count = run_iteration(g, &colony, 0, num_nodes-1, it, stdout);
        // Calculate ratio of ants on the optimal path
        double ratio = (double)optimal_count / colony.num_ants;
        // If 90% or more ants chose the optimal path, increment consecutive counter
        if (ratio >= 0.9) {
            consecutive++;
            // If this happens 3 times in a row, mark convergence
            if (consecutive == 3 && convergence_iter == -1) {
                convergence_iter = it - 2; // record the first of the 3 consecutive iterations
            }
        } else {
            consecutive = 0; // reset if condition not met
        }
    }

    // End timing
    clock_t end = clock();
    double runtime = (double)(end - start) / CLOCKS_PER_SEC;
    // Inspect pheromone levels on key edges
    double shortcut_pher = g->edges[0][num_nodes-1].pheromone; // shortcut edge pheromone
    double first_edge_pher = g->edges[0][1].pheromone; // first edge pheromone
    // Print a single clean summary line of results
    if (convergence_iter == -1) {
        printf("Nodes=%d | Ants=%d | Evap=%.2f | PherW=%.2f | DistW=%.2f | Shortcut=%.2f | Edge01=%.2f | Runtime=%.3fs | ConvergenceIter=No\n",
            num_nodes, num_ants, evap, pher_w, dist_w,
            shortcut_pher, first_edge_pher, runtime);
    } else {
        printf("Nodes=%d | Ants=%d | Evap=%.2f | PherW=%.2f | DistW=%.2f | Shortcut=%.2f | Edge01=%.2f | Runtime=%.3fs | ConvergenceIter=%d\n",
            num_nodes, num_ants, evap, pher_w, dist_w,
            shortcut_pher, first_edge_pher, runtime, convergence_iter);
    }
    // Free memory for the graph
    free_ant_graph(g);
}

int main() {
    printf("ACO Analysis Results:\n");
    // Parameter sensitivity
    run_analysis(4, 0.1, 1.0, 2.0, 10, 10);
    run_analysis(4, 0.3, 1.0, 2.0, 10, 10);
    run_analysis(4, 0.1, 2.0, 1.0, 10, 10);
    // Scalability
    run_analysis(10, 0.1, 1.0, 2.0, 20, 10);
    run_analysis(20, 0.1, 1.0, 2.0, 20, 10);
    // Scalability with different ant counts
    run_analysis(20, 0.1, 1.0, 2.0, 20, 5); 
    run_analysis(20, 0.1, 1.0, 2.0, 20, 10);
    run_analysis(20, 0.1, 1.0, 2.0, 20, 20);
    run_analysis(20, 0.1, 1.0, 2.0, 20, 50);

    return 0;
}
