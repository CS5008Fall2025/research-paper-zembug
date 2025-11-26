#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#include "aco.h"
#include "ant_graph.h"


/* Choose the next node for an ant using pheromone strength and edge distance
 * @param g The graph
 * @param current The current node of the ant
 * @param previous The node visited immediately before (to prevent backtracking)
 * @param visited Array indicating which nodes have been visited
 * @param num_nodes Total number of nodes in the graph
 * @param colony Parameters controlling pheromone vs distance weighting
 * @return Index of the chosen next node, or -1 if no valid move exists
 */
int pick_next_node(AntGraph* g, int current, int previous, int* visited, int num_nodes, AntColony* colony) {
    // allocate an array to store "appeal" values for each possible next node
    double *appeal = malloc(num_nodes * sizeof(double));
    if (!appeal) return -1; // return failure if allocation fails
    double total = 0.0; // total appeal across all candidate nodes
    // compute appeal for each possible next node
    for (int j = 0; j < num_nodes; j++) {
        if (!g->edges[current][j].exists) {
            appeal[j] = 0; // no edge → no appeal
            continue;
        }
        if (j == previous) {
            appeal[j] = 0; // prevent immediate backtracking to the previous node
            continue;
        }
        // pheromone level on edge (current → j)
        double pher = g->edges[current][j].pheromone;
        // heuristic: inverse of edge weight (shorter edges are more appealing)
        double heur = 1.0 / g->edges[current][j].weight;
        // combine pheromone and heuristic using colony parameters
        appeal[j] = pow(pher, colony->pheromone_weight) * pow(heur, colony->distance_weight);
        if (visited[j]) {
            appeal[j] *= 0.1; // reduce appeal if node has already been visited
        }
        total += appeal[j]; // accumulate total appeal
    }
    // if no valid moves (total appeal = 0), return failure
    if (total == 0.0) {
        free(appeal);
        return -1;
    }
    // roulette‑wheel selection: pick a random threshold in [0, total]
    double r = ((double)rand() / RAND_MAX) * total;
    double cumulative = 0.0;
    // walk through nodes, accumulating appeal until threshold is reached
    for (int j = 0; j < num_nodes; j++) {
        cumulative += appeal[j];
        if (cumulative >= r) {
            free(appeal);
            return j; // select this node
        }
    }
    // fallback: if roulette selection fails, pick the node with highest appeal
    int best = -1;
    double best_appeal = -1;
    for (int j = 0; j < num_nodes; j++) {
        if (appeal[j] > best_appeal) {
            best_appeal = appeal[j];
            best = j;
        }
    }
    free(appeal); // free allocated memory
    return best;  // return the chosen node
}


/* Construct a path from start to end by repeatedly calling pick_next_node
 * @param g The graph
 * @param start The starting node
 * @param end The ending node
 * @param num_nodes Total number of nodes in the graph
 * @param path Array to store the constructed path
 * @param path_length Pointer to store the length of the constructed path
 * @param colony Parameters controlling pheromone vs distance weighting
 */
void build_path(AntGraph* g, int start, int end, int num_nodes, int* path, int* path_length, AntColony* colony){
    // track visited nodes (0 = unvisited, 1 = visited)
    int* visited = calloc(num_nodes, sizeof(int)); 
    int current = start;     // current node the ant is on
    int previous = -1;       // previous node (none at start)
    int idx = 0;             // index into the path array
    // allow at most num_nodes*5 steps to avoid infinite loops
    int max_steps = num_nodes * 5;
    // record the starting node in the path
    path[idx++] = current;
    visited[current] = 1; // mark start as visited
    // walk until max_steps or until reaching the end node
    for (int steps = 0; steps < max_steps; steps++) {
        if (current == end)
            break; // stop if destination reached
        // choose the next node based on pheromone and distance
        int next = pick_next_node(g, current, previous, visited, num_nodes, colony);
        if (next == -1) {
            // no valid move path failed
            *path_length = 0;
            free(visited);
            return;
        }
        // record the chosen node in the path
        path[idx++] = next;
        visited[next] = 1;   // mark as visited
        previous = current;  // update previous node
        current = next;      // move to next node
    }
    // if we ended at the destination, record path length
    if (current == end)
        *path_length = idx;
    else
        *path_length = 0; // otherwise, path is invalid
    free(visited); // free memory for visited array
}


// log one ant's path to console and logfile
static void log_ant_path(FILE* logfile, int* path, int path_length, int ant_id) {
    //printf("  Ant %d path:", ant_id);
    fprintf(logfile, "  Ant %d path:", ant_id);
    for (int i = 0; i < path_length; i++) {
        //printf(" %d", path[i]);
        fprintf(logfile, " %d", path[i]);
    }
    //printf("\n");
    fprintf(logfile, "\n");
}


// deposit pheromones along a path, with clamping
static void deposit_pheromones(AntGraph* g, int* path, int path_length, AntColony* colony) {
    double deposit = colony->deposit_amount / path_length;
    for (int i = 0; i < path_length - 1; i++) {
        int u = path[i];
        int v = path[i + 1];
        g->edges[u][v].pheromone += deposit;
        g->edges[v][u].pheromone += deposit;

        // clamp pheromone values
        if (g->edges[u][v].pheromone < 0.01) g->edges[u][v].pheromone = 0.01;
        if (g->edges[u][v].pheromone > 10.0) g->edges[u][v].pheromone = 10.0;
        if (g->edges[v][u].pheromone < 0.01) g->edges[v][u].pheromone = 0.01;
        if (g->edges[v][u].pheromone > 10.0) g->edges[v][u].pheromone = 10.0;
    }
}

// Update the best path if the new one is shorter
void update_best_paths(const int *path, int path_length, int *best_path, int *best_length, AntColony *colony) {
    // Only update if this path is better
    if (path_length < *best_length) {
        // Cap the copy length to avoid overflow
        int len = (path_length < colony->max_steps) ? path_length : colony->max_steps;

        memcpy(best_path, path, len * sizeof(int));
        *best_length = len;
    }
}


// evaporate pheromones across the whole graph
static void evaporate_pheromones(AntGraph* g, AntColony* colony) {
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->edges[i][j].exists) {
                g->edges[i][j].pheromone *= (1.0 - colony->evaporation_rate);
                if (g->edges[i][j].pheromone < 0.01) g->edges[i][j].pheromone = 0.01;
                if (g->edges[i][j].pheromone > 10.0) g->edges[i][j].pheromone = 10.0;
            }
        }
    }
}

// compute the weighted cost of a path
static double compute_path_cost(AntGraph* g, const int* path, int path_length) {
    double cost = 0.0;
    for (int i = 0; i < path_length - 1; i++) {
        int u = path[i];
        int v = path[i+1];
        cost += g->edges[u][v].weight;
    }
    return cost;
}



static void log_iteration_best(FILE* logfile, int iteration,
                               AntGraph* g, int* best_path, int best_length) {
    double cost = compute_path_cost(g, best_path, best_length);

    // existing logs...
    printf("  Best path node count this iteration: %d\n", best_length);
    fprintf(logfile, "  Best path node count this iteration: %d\n", best_length);

    printf("  Best path weighted cost this iteration: %.2f\n", cost);
    fprintf(logfile, "  Best path weighted cost this iteration: %.2f\n", cost);

    printf("  Best path:");
    fprintf(logfile, "  Best path:");
    for (int i = 0; i < best_length; i++) {
        printf(" %d", best_path[i]);
        fprintf(logfile, " %d", best_path[i]);
    }
    printf("\n");
    fprintf(logfile, "\n");

    // normalized cost vs chain baseline (edit baseline if needed)
    double chain_cost = 1.1 * (g->num_nodes - 1);
    double norm = cost / chain_cost;

    // CSV append
    FILE* csv = fopen("convergence.csv", "a");
    if (csv) {
        fprintf(csv, "%d,%d,%.2f,%.4f\n",
                iteration + 1, best_length, cost, norm);
        fclose(csv);
    }
}




int run_iteration(AntGraph* g, AntColony* colony, int start, int end, int iteration, FILE* logfile) {
    printf("Iteration %d:\n", iteration + 1);
    fprintf(logfile, "Iteration %d:\n", iteration + 1);

    int optimal_count = 0;
    int max_steps = g->num_nodes * 5;
    colony->max_steps = max_steps;


    int *best_path = malloc(max_steps * sizeof(int));
    if (!best_path) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }
    int best_length = INT_MAX;

    for (int a = 0; a < colony->num_ants; a++) {
        int *path = malloc(max_steps * sizeof(int));
        int path_length = 0;
        if (!path) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }

        build_path(g, start, end, g->num_nodes, path, &path_length, colony);

        log_ant_path(logfile, path, path_length, a+1);

        if (path_length > 0 && path[path_length - 1] == end) {
            deposit_pheromones(g, path, path_length, colony);
            update_best_paths(path, path_length, best_path, &best_length, colony);
        }

        free(path);
    }

    // evaporate pheromones
    evaporate_pheromones(g, colony);

    // log iteration best
    if (best_length < INT_MAX) {
        log_iteration_best(logfile, iteration, g, best_path, best_length);    
    }

    free(best_path);  // <-- free AFTER last use

    return optimal_count;
}






// log the global best path after all iterations
static void log_global_best(FILE* logfile, AntColony* colony) {
    if (colony->global_best_length < INT_MAX) {
        //printf("Global best path length: %d\n", colony->global_best_length);
        fprintf(logfile, "Global best path length: %d\n", colony->global_best_length);

        //printf("Global best path:");
        fprintf(logfile, "Global best path:");
        for (int i = 0; i < colony->global_best_length; i++) {
            //printf(" %d", colony->global_best_path[i]);
            fprintf(logfile, " %d", colony->global_best_path[i]);
        }
        //printf("\n");
        fprintf(logfile, "\n");
    }
}



void run_aco(AntGraph* g, AntColony* colony, int start, int end, int iterations, FILE* logfile) {
    printf("Starting ACO with %d ants, %d iterations.\n", colony->num_ants, iterations);
    fprintf(logfile, "Starting ACO with %d ants, %d iterations.\n", colony->num_ants, iterations);

    FILE* csv = fopen("convergence.csv", "w");
    if (csv) {
        fprintf(csv, "Iteration,BestPathLength,BestPathCost,NormBestCost\n");
        fclose(csv);
    }
    colony->global_best_length = INT_MAX;

    for (int iteration = 0; iteration < iterations; iteration++) {
        run_iteration(g, colony, start, end, iteration, logfile);
    }

    printf("ACO finished after %d iterations.\n", iterations);
    fprintf(logfile, "ACO finished after %d iterations.\n", iterations);

    // use helper for global best logging
    log_global_best(logfile, colony);
}

  





