#include <stdio.h>
#include <time.h>
#include <stdlib.h>   // for free()
#include "ant_graph.h"
#include "aco.h"

// declare run_iteration so we can call the one in aco.c
int run_iteration(AntGraph* g, AntColony* colony, int start, int end, int iteration, FILE* logfile);

// Analysis harness with convergence tracking
void run_analysis(int num_nodes, double evap, double pher_w, double dist_w, int iterations, int num_ants) {
    AntGraph* g = create_ant_graph(num_nodes);

    // Build a line graph with a shortcut
    for (int i = 0; i < num_nodes-1; i++) {
        add_edge(g, i, i+1, 1.0);
    }
    add_edge(g, 0, num_nodes-1, 2.0); // shortcut edge

    AntColony colony = {
        .num_ants = num_ants,
        .pheromone_weight = pher_w,
        .distance_weight = dist_w,
        .evaporation_rate = evap,
        .deposit_amount = 1.0
    };

    clock_t start = clock();

    int convergence_iter = -1, consecutive = 0;
    for (int it = 1; it <= iterations; it++) {
        int optimal_count = run_iteration(g, &colony, 0, num_nodes-1, it, stdout);
        double ratio = (double)optimal_count / colony.num_ants;

        if (ratio >= 0.9) {
            consecutive++;
            if (consecutive == 3 && convergence_iter == -1) {
                convergence_iter = it - 2; // first of the 3 consecutive
            }
        } else {
            consecutive = 0;
        }
    }

    clock_t end = clock();
    double runtime = (double)(end - start) / CLOCKS_PER_SEC;
    double shortcut_pher = g->edges[0][num_nodes-1].pheromone;
    double first_edge_pher = g->edges[0][1].pheromone;

    // Single clean summary line
    if (convergence_iter == -1) {
        printf("Nodes=%d | Ants=%d | Evap=%.2f | PherW=%.2f | DistW=%.2f | Shortcut=%.2f | Edge01=%.2f | Runtime=%.3fs | ConvergenceIter=No\n",
            num_nodes, num_ants, evap, pher_w, dist_w,
            shortcut_pher, first_edge_pher, runtime);
    } else {
        printf("Nodes=%d | Ants=%d | Evap=%.2f | PherW=%.2f | DistW=%.2f | Shortcut=%.2f | Edge01=%.2f | Runtime=%.3fs | ConvergenceIter=%d\n",
            num_nodes, num_ants, evap, pher_w, dist_w,
            shortcut_pher, first_edge_pher, runtime, convergence_iter);
    }


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
