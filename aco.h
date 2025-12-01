// aco.h
#ifndef ACO_H
#define ACO_H

#include <stdio.h>
#include "ant_graph.h"

typedef struct {
    int num_ants;
    double alpha;
    double beta;
    double evaporation_rate;
    double deposit_amount;

    int* global_best_path;
    int global_best_length;
    int global_best_capacity;

    int prevent_backtracking;
    int max_steps;
    int use_global_best_update;
} AntColony;

void run_aco(AntGraph* g, AntColony* colony, int start, int end, int iterations, FILE* logfile);
int pick_next_node(AntGraph* g, int current, int previous, int* visited, int num_nodes, AntColony* colony);
void build_path(AntGraph* g, int start, int end, int num_nodes, int* path, int* path_length, AntColony* colony);
int run_iteration(AntGraph* g, AntColony* colony, int start, int end, int iteration, FILE* logfile);

#endif
