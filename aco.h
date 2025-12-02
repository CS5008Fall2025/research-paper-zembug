// aco.h
#ifndef ACO_H
#define ACO_H

#include <stdio.h>
#include "ant_graph.h"

// Definition of the AntColony structure
typedef struct {
    int num_ants; // Number of ants in the colony
    double alpha; // Influence of pheromone trails
    double beta; // Influence of heuristic (edge length)
    double evaporation_rate; // Rate pheromone trails fade
    double deposit_amount; // Amount of pheromone deposited by each ant

    int* global_best_path; // Array storing best path found so far
    int global_best_length; // Length of best path
    int global_best_capacity; // Capacity of the global best path array

    int prevent_backtracking; // Flag to prevent ants from immediately returning to previous node
    int max_steps; // Maximum steps an ant can take in a single path
    int use_global_best_update; // Flag to control whether global best is reinforced each iteration
} AntColony;

// Run the Ant Colony Optimization algorithm.
void run_aco(AntGraph* g, AntColony* colony, int start, int end, int iterations, FILE* logfile);
// Pick the next node for an ant to move to.
int pick_next_node(AntGraph* g, int current, int previous, int* visited, int num_nodes, AntColony* colony);
// Build a path for an ant from start to end.
void build_path(AntGraph* g, int start, int end, int num_nodes, int* path, int* path_length, AntColony* colony);
// Run a single iteration of the ACO algorithm.
int run_iteration(AntGraph* g, AntColony* colony, int start, int end, int iteration, FILE* logfile);

#endif
