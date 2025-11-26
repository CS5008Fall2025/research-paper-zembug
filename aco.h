#ifndef ACO_H
#define ACO_H

#include <stdio.h>
#include "ant_graph.h"

// Structure representing the parameters of the Ant Colony Optimization algorithm
typedef struct {
    int num_ants; // number of ants in the colony
    double pheromone_weight; // influence of pheromone trails
    double distance_weight; // influence of edge distance (heuristic)
    double evaporation_rate; // rate at which pheromone evaporates
    double deposit_amount; // pheromone deposited per ant per step

    // global best path tracking
    int global_best_path[100]; // array to store the best path found
    int global_best_length; // length of the best path found
    int max_steps; // maximum allowed steps in a path
} AntColony;


// Execute the Ant Colony Optimization loop on the given graph
void run_aco(AntGraph* g, AntColony* colony, int start, int end, int iterations, FILE* logfile);

// Pick the next node for an ant to move to based on pheromone and distance
int pick_next_node(AntGraph* g, int current, int previous, int* visited, int num_nodes, AntColony* colony);

// Build a path from start to end for a single ant
void build_path(AntGraph* g, int start, int end, int num_nodes, int* path, int* path_length, AntColony* colony);

// Run a single iteration of the ACO algorithm
int run_iteration(AntGraph* g, AntColony* colony, int start, int end, int iteration, FILE* logfile);

#endif