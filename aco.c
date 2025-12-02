// aco.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "aco.h"
#include "ant_graph.h"


/**
 * Decide which node an ant should move to next.
 * @param g Pointer to the graph structure.
 * @param current Index of the current node.
 * @param previous Index of the previous node (to prevent backtracking).
 * @param visited Array indicating which nodes have been visited.
 * @param num_nodes Total number of nodes in the graph.
 * @param colony Pointer to the ant colony (contains parameters like alpha, beta, etc.).
 * @return Index of the next node to move to, or -1 if no valid move exists.
 */
int pick_next_node(AntGraph* g, int current, int previous, int* visited, int num_nodes, AntColony* colony) {
    double exploration_prob = 0.05; // 5% chance the ant ignores pheromones and picks randomly
    // Collect all valid neighbors of the current node
    int valid_count = 0; // how many valid neighbors we find
    int valid_nodes[num_nodes]; // store their indices
    for (int j = 0; j < num_nodes; j++) {
        // Skip if: no edge exists, backtracking is prevented and j==previous, or node already visited
        if (!g->edges[current][j].exists || (colony->prevent_backtracking && j == previous) || visited[j])
            continue;
        valid_nodes[valid_count++] = j; // add neighbor to the list
    }
    // Random exploration: with 5% probability, pick a random neighbor from the valid list
    if (valid_count > 0 && ((double)rand() / RAND_MAX) < exploration_prob) {
        return valid_nodes[rand() % valid_count]; // choose one at random
    }
    // Otherwise, calculate probability based on pheromone and heuristic (distance)
    double* appeal = malloc(num_nodes * sizeof(double)); // array to store appeal values
    if (!appeal) return -1; // if memory allocation fails, return -1

    double total = 0.0; // sum of all appeal values
    for (int j = 0; j < num_nodes; j++) {
        // Skip invalid neighbors (same checks as before)
        if (!g->edges[current][j].exists || (colony->prevent_backtracking && j == previous) || visited[j]) {
            appeal[j] = 0; // no appeal for invalid moves
            continue;
        }
        double pher = g->edges[current][j].pheromone; // pheromone level on edge
        double heur = 1.0 / g->edges[current][j].weight; // heuristic: inverse of edge weight (shorter = better)
        // Appeal = pheromone^alpha * heuristic^beta
        appeal[j] = pow(pher, colony->alpha) * pow(heur, colony->beta);
        total += appeal[j]; // add to total appeal
    }
    // If no valid moves (total appeal = 0), free memory and return -1
    if (total == 0.0) {
        free(appeal);
        return -1;
    }
    // Roulette wheel selection: pick a neighbor proportional to its appeal
    double r = ((double)rand() / RAND_MAX) * total; // random threshold between 0 and total
    double cumulative = 0.0;
    for (int j = 0; j < num_nodes; j++) {
        cumulative += appeal[j]; // accumulate appeal
        if (cumulative >= r) { // when threshold is crossed, pick this node
            free(appeal);
            return j;
        }
    }
    // If roulette wheel fails, pick the node with highest appeal
    int best = -1;
    double best_appeal = -1;
    for (int j = 0; j < num_nodes; j++) {
        if (appeal[j] > best_appeal) {
            best_appeal = appeal[j];
            best = j;
        }
    }
    free(appeal); // free allocated memory
    return best;  // return the best node found
}


/**
 * Build a path for a single ant from start to end.
 * @param g Pointer to the graph structure.
 * @param start Index of the starting node.
 * @param end Index of the target node.
 * @param num_nodes Total number of nodes in the graph.
 * @param path Array to store the sequence of nodes visited.
 * @param path_length Pointer to an integer where the final path length is written.
 * @param colony Pointer to the ant colony (contains parameters like alpha, beta, etc.).
 */
void build_path(AntGraph* g, int start, int end, int num_nodes, int* path, int* path_length, AntColony* colony) {
    // Track which nodes have been visited (initialized to 0)
    int* visited = calloc(num_nodes, sizeof(int));
    int current = start; // current node
    int previous = -1; // previous node (none at the start)
    int idx = 0; // index in the path array
    int max_steps = num_nodes; // maximum steps allowed (avoid infinite loops)
    // Place the starting node into the path
    path[idx++] = current;
    visited[current] = 1;
    // Walk through the graph until reaching the end or hitting max steps
    for (int steps = 0; steps < max_steps; steps++) {
        if (current == end) break; // stop if target reached
        // Decide the next node using pheromone + heuristic rules
        int next = pick_next_node(g, current, previous, visited, num_nodes, colony);
        // If no valid move is found, mark path as invalid and exit
        if (next == -1) {
            *path_length = 0;
            free(visited);
            return;
        }
        // Add the chosen node to the path
        path[idx++] = next;
        visited[next] = 1;
        // Update current and previous for the next step
        previous = current;
        current = next;
    }
    // If we ended at the target, record the path length; otherwise mark as invalid
    *path_length = (current == end) ? idx : 0;
    // Free memory used for visited tracking
    free(visited);
}


/** Compute the total cost of a given path.
 * @param g Pointer to the graph structure.
 * @param path Array containing the sequence of nodes visited.
 * @param path_length Number of nodes in the path.
 * @return Total cost (sum of edge weights) of the path.
 */
static double compute_path_cost(AntGraph* g, const int* path, int path_length) {
    double cost = 0.0; // total cost accumulator
    // Sum the weights of each edge in the path
    for (int i = 0; i < path_length - 1; i++) {
        cost += g->edges[path[i]][path[i + 1]].weight;
    }
    return cost; // return the computed cost
}


/**
 * Log one ant's path to both console and a logfile.
 * @param logfile File stream to write the path into.
 * @param path Array containing the sequence of nodes visited.
 * @param path_length Number of nodes in the path.
 * @param ant_id Identifier of the ant whose path is being logged.
 */
static void log_ant_path(FILE* logfile, int* path, int path_length, int ant_id) {
    // Print header with ant ID
    printf("  Ant %d path:", ant_id);
    fprintf(logfile, "  Ant %d path:", ant_id);
    // Print each node in the path
    for (int i = 0; i < path_length; i++) {
        printf(" %d", path[i]);
        fprintf(logfile, " %d", path[i]);
    }
    printf("\n");
    fprintf(logfile, "\n");
}


/**
 * Deposit pheromones along the path taken by an ant.
 * @param g Pointer to the graph structure.
 * @param path Array containing the sequence of nodes visited.
 * @param path_length Number of nodes in the path.
 * @param colony Pointer to the ant colony (contains deposit amount, etc.).
 */
static void deposit_pheromones(AntGraph* g, int* path, int path_length, AntColony* colony) {
    // Compute the total cost (length) of the path
    double L = compute_path_cost(g, path, path_length);
    // Amount of pheromone to deposit is inversely proportional to path cost
    double deposit = colony->deposit_amount / L;
    // Walk through each edge in the path
    for (int i = 0; i < path_length - 1; i++) {
        int u = path[i]; // current node
        int v = path[i + 1]; // next node
        // Add pheromone to both directions of the edge
        g->edges[u][v].pheromone += deposit;
        g->edges[v][u].pheromone += deposit;
        // Clamp pheromone values to stay within reasonable bounds
        if (g->edges[u][v].pheromone < 0.01) g->edges[u][v].pheromone = 0.01; // minimum floor
        if (g->edges[u][v].pheromone > 10.0) g->edges[u][v].pheromone = 10.0; // maximum cap
    }
}


/**
 * Evaporate pheromones on all edges in the graph.
 * @param g Pointer to the graph structure.
 * @param colony Pointer to the ant colony (contains evaporation rate).
 */
static void evaporate_pheromones(AntGraph* g, AntColony* colony) {
    // Loop through all edges in the graph
    for (int i = 0; i < g->num_nodes; i++) {
        // Evaporate pheromone on each edge connected to node i
        for (int j = 0; j < g->num_nodes; j++) {
            // Only evaporate if the edge exists
            if (g->edges[i][j].exists) {
                g->edges[i][j].pheromone *= (1.0 - colony->evaporation_rate);
                if (g->edges[i][j].pheromone < 0.01) g->edges[i][j].pheromone = 0.01;
            }
        }
    }
}


/** Log the best path found in the current iteration to console, logfile, and CSV.
 * @param logfile File stream to write logs into.
 * @param iteration Current iteration number.
 * @param g Pointer to the graph structure.
 * @param best_path Array containing the best path found this iteration.
 * @param best_length Length of the best path.
 * @param colony Pointer to the ant colony (for global best info).
 */
static void log_iteration_best(FILE* logfile, int iteration, AntGraph* g, int* best_path, int best_length, AntColony* colony) {
    // Compute the cost of the best path
    double cost = compute_path_cost(g, best_path, best_length);
    
    // Log to console and logfile
    printf("  Best path node count this iteration: %d\n", best_length);
    fprintf(logfile, "  Best path node count this iteration: %d\n", best_length);

    printf("  Best path weighted cost this iteration: %.2f\n", cost);
    fprintf(logfile, "  Best path weighted cost this iteration: %.2f\n", cost);

    printf("  Best path:");
    fprintf(logfile, "  Best path:");
    // Print each node in the best path
    for (int i = 0; i < best_length; i++) {
        printf(" %d", best_path[i]);
        fprintf(logfile, " %d", best_path[i]);
    }
    printf("\n");
    fprintf(logfile, "\n");
    // Log to CSV file
    double chain_cost = 1.1 * (g->num_nodes - 1); // expected cost of the chain path
    double norm = cost / chain_cost; // normalized cost
    // Append to convergence.csv
    FILE* csv = fopen("convergence.csv", "a");
    if (csv) {
        double global_cost = compute_path_cost(g, colony->global_best_path, colony->global_best_length); // global best cost
        double improvement = chain_cost / cost; // improvement factor this iteration
        // Write a row with iteration data
        fprintf(csv, "%d,%d,%.2f,%.4f,%d,%.2f,%.4f,%.2f,%.2f,%.2f,%.2f\n", iteration + 1, best_length, 
            cost, norm, colony->global_best_length, global_cost, improvement, colony->alpha, colony->beta,
            colony->evaporation_rate, colony->deposit_amount);
        fclose(csv);
    }
}


/** Run a single iteration of the ACO algorithm.
 * @param g Pointer to the graph structure.
 * @param colony Pointer to the ant colony (contains parameters like number of ants, etc.).
 * @param start Index of the starting node.
 * @param end Index of the target node.
 * @param iteration Current iteration number.
 * @param logfile File stream to write logs into.
 * @return Number of ants that found the optimal path (for convergence tracking).
 */
int run_iteration(AntGraph* g, AntColony* colony, int start, int end, int iteration, FILE* logfile) {
    if (!g || !colony) return 0;

    printf("Iteration %d:\n", iteration + 1);
    fprintf(logfile, "Iteration %d:\n", iteration + 1);

    int max_steps = g->num_nodes; // set max steps to number of nodes
    colony->max_steps = max_steps; // update colony max steps

    int *best_path = malloc(max_steps * sizeof(int)); // allocate for best path
    if (!best_path) { fprintf(stderr, "Memory allocation failed\n"); exit(1); } // check allocation
    int best_length = INT_MAX; // initialize best length to max

    // Track how many ants found the optimal path
    for (int a = 0; a < colony->num_ants; a++) {
        int *path = malloc(max_steps * sizeof(int)); // allocate for this ant's path
        if (!path) { fprintf(stderr, "Memory allocation failed\n"); exit(1); } // check allocation
        int path_length = 0; // initialize path length
        // Build path for this ant
        build_path(g, start, end, g->num_nodes, path, &path_length, colony); 

        if (path_length > 0 && path[path_length - 1] == end) {
            // Track the iteration best path
            if (path_length < best_length) { 
                best_length = path_length; // update best length
                memcpy(best_path, path, best_length * sizeof(int)); // copy best path
            }
            // Update global best path
            if (path_length < colony->global_best_length) {
                int copy_len = (path_length > colony->global_best_capacity) ? colony->global_best_capacity : path_length;
                memcpy(colony->global_best_path, path, copy_len * sizeof(int)); // copy global best path
                colony->global_best_length = path_length;
            }
        }
        // Log this ant's path
        log_ant_path(logfile, path, path_length, a);
        free(path);
    }
    // Deposit pheromones for iteration best
    if (best_length < INT_MAX) {
        deposit_pheromones(g, best_path, best_length, colony); // iteration best
    }
    // Deposit pheromones for global best
    if (colony->global_best_length < INT_MAX) {
        deposit_pheromones(g, colony->global_best_path, colony->global_best_length, colony); // global best
    }
    // Evaporate pheromones after deposition
    evaporate_pheromones(g, colony);
    // Log iteration best to CSV/console
    if (best_length < INT_MAX) {
        log_iteration_best(logfile, iteration, g, best_path, best_length, colony);
    }
    free(best_path);
    return 0;
}



/** Log the global best path found by the colony.
 * @param logfile File stream to write logs into.
 * @param colony Pointer to the ant colony (contains global best info).
 */
static void log_global_best(FILE* logfile, AntColony* colony) {
    if (colony->global_best_length < INT_MAX) { // check if a global best exists
        printf("Global best path length: %d\n", colony->global_best_length);
        fprintf(logfile, "Global best path length: %d\n", colony->global_best_length);

        printf("Global best path:");
        fprintf(logfile, "Global best path:");
        // Print each node in the global best path
        for (int i = 0; i < colony->global_best_length; i++) {
            printf(" %d", colony->global_best_path[i]);
            fprintf(logfile, " %d", colony->global_best_path[i]);
        }
        printf("\n");
        fprintf(logfile, "\n");
    }
}


/** Run the Ant Colony Optimization algorithm.
 * @param g Pointer to the graph structure.
 * @param colony Pointer to the ant colony (contains parameters like number of ants, etc.).
 * @param start Index of the starting node.
 * @param end Index of the target node.
 * @param iterations Number of iterations to run.
 * @param logfile File stream to write logs into.
 */
void run_aco(AntGraph* g, AntColony* colony, int start, int end, int iterations, FILE* logfile) {
    // Announce start of ACO to console and logfile
    printf("Starting ACO with %d ants, %d iterations.\n", colony->num_ants, iterations);
    fprintf(logfile, "Starting ACO with %d ants, %d iterations.\n", colony->num_ants, iterations);
    // Create CSV file to record convergence data, will overwrite existing
    FILE* csv = fopen("convergence.csv", "w");
    if (csv) {
        // Write CSV header row for column names
        fprintf(csv, "Iteration,BestPathLength,BestPathCost,NormBestCost,GlobalBestLength,GlobalBestCost,"
            "ImprovementFactor,Alpha,Beta,Evaporation,Deposit\n");
        fclose(csv);
    }
    colony->global_best_length = INT_MAX; // initialize global best length
    // Allocate memory for global best path
    for (int iteration = 0; iteration < iterations; iteration++) {
        run_iteration(g, colony, start, end, iteration, logfile);
    }
    // Announce completion of ACO to console and logfile
    printf("ACO finished after %d iterations.\n", iterations);
    fprintf(logfile, "ACO finished after %d iterations.\n", iterations);

    // use helper for global best logging
    log_global_best(logfile, colony);

    // Final entry to convergence CSV
    csv = fopen("convergence.csv", "a");
    if (csv) {
        // Calculate final metrics for logging
        double global_cost = compute_path_cost(g, colony->global_best_path, colony->global_best_length);
        double chain_cost = 1.1 * (g->num_nodes - 1); // expected cost of the chain path
        double norm = global_cost / chain_cost; // normalized cost
        double improvement = chain_cost / global_cost; // overall improvement factor
        // Write final summary row
        fprintf(csv, "Final,%d,%.2f,%.4f,%d,%.2f,%.4f,%.2f,%.2f,%.2f,%.2f\n", colony->global_best_length,
                global_cost, norm, colony->global_best_length, global_cost, improvement, colony->alpha,
                colony->beta, colony->evaporation_rate, colony->deposit_amount);
        fclose(csv);
    }
}