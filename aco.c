// aco.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "aco.h"
#include "ant_graph.h"

int pick_next_node(AntGraph* g, int current, int previous, int* visited, int num_nodes, AntColony* colony) {
    double* appeal = malloc(num_nodes * sizeof(double));
    if (!appeal) return -1;

    double total = 0.0;

    for (int j = 0; j < num_nodes; j++) {
        if (!g->edges[current][j].exists || (colony->prevent_backtracking && j == previous) || visited[j]) {
            appeal[j] = 0;
            continue;
        }

        double pher = g->edges[current][j].pheromone;
        double heur = 1.0 / g->edges[current][j].weight;
        appeal[j] = pow(pher, colony->alpha) * pow(heur, colony->beta);
        total += appeal[j];
    }

    if (total == 0.0) {
        free(appeal);
        return -1;
    }

    double r = ((double)rand() / RAND_MAX) * total;
    double cumulative = 0.0;

    for (int j = 0; j < num_nodes; j++) {
        cumulative += appeal[j];
        if (cumulative >= r) {
            free(appeal);
            return j;
        }
    }

    // fallback
    int best = -1;
    double best_appeal = -1;
    for (int j = 0; j < num_nodes; j++) {
        if (appeal[j] > best_appeal) {
            best_appeal = appeal[j];
            best = j;
        }
    }

    free(appeal);
    return best;
}

void build_path(AntGraph* g, int start, int end, int num_nodes, int* path, int* path_length, AntColony* colony) {
    int* visited = calloc(num_nodes, sizeof(int));
    int current = start;
    int previous = -1;
    int idx = 0;
    int max_steps = num_nodes;
    path[idx++] = current;
    visited[current] = 1;

    for (int steps = 0; steps < max_steps; steps++) {
        if (current == end) break;
        int next = pick_next_node(g, current, previous, visited, num_nodes, colony);
        if (next == -1) {
            *path_length = 0;
            free(visited);
            return;
        }
        path[idx++] = next;
        visited[next] = 1;
        previous = current;
        current = next;
    }

    *path_length = (current == end) ? idx : 0;
    free(visited);
}

static double compute_path_cost(AntGraph* g, const int* path, int path_length) {
    double cost = 0.0;
    for (int i = 0; i < path_length - 1; i++) {
        cost += g->edges[path[i]][path[i + 1]].weight;
    }
    return cost;
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

static void deposit_pheromones(AntGraph* g, int* path, int path_length, AntColony* colony) {
    double L = compute_path_cost(g, path, path_length);
    double deposit = colony->deposit_amount / L;

    for (int i = 0; i < path_length - 1; i++) {
        int u = path[i];
        int v = path[i + 1];
        g->edges[u][v].pheromone += deposit;
        g->edges[v][u].pheromone += deposit;
        if (g->edges[u][v].pheromone < 0.01) g->edges[u][v].pheromone = 0.01;
        if (g->edges[u][v].pheromone > 10.0) g->edges[u][v].pheromone = 10.0;
    }
}

static void evaporate_pheromones(AntGraph* g, AntColony* colony) {
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->edges[i][j].exists) {
                g->edges[i][j].pheromone *= (1.0 - colony->evaporation_rate);
                if (g->edges[i][j].pheromone < 0.01) g->edges[i][j].pheromone = 0.01;
            }
        }
    }
}

static void log_iteration_best(FILE* logfile, int iteration,
                               AntGraph* g, int* best_path, int best_length,
                               AntColony* colony) {
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

    // normalized cost vs chain baseline
    double chain_cost = 1.1 * (g->num_nodes - 1);
    double norm = cost / chain_cost;

    // CSV append
    FILE* csv = fopen("convergence.csv", "a");
    if (csv) {
        double global_cost = compute_path_cost(g, colony->global_best_path,
                                               colony->global_best_length);
        double improvement = chain_cost / cost;

        fprintf(csv, "%d,%d,%.2f,%.4f,%d,%.2f,%.4f,%.2f,%.2f,%.2f,%.2f\n",
                iteration + 1,
                best_length,
                cost,
                norm,
                colony->global_best_length,
                global_cost,
                improvement,
                colony->alpha,
                colony->beta,
                colony->evaporation_rate,
                colony->deposit_amount);
        fclose(csv);
    }
}


int run_iteration(AntGraph* g, AntColony* colony, int start, int end, int iteration, FILE* logfile) {
    if (!g || !colony) return 0;

    printf("Iteration %d:\n", iteration + 1);
    fprintf(logfile, "Iteration %d:\n", iteration + 1);

    int max_steps = g->num_nodes;
    colony->max_steps = max_steps;

    int *best_path = malloc(max_steps * sizeof(int));
    if (!best_path) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }
    int best_length = INT_MAX;

    for (int a = 0; a < colony->num_ants; a++) {
        int *path = malloc(max_steps * sizeof(int));
        if (!path) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }
        int path_length = 0;

        build_path(g, start, end, g->num_nodes, path, &path_length, colony);

        if (path_length > 0 && path[path_length - 1] == end) {
            deposit_pheromones(g, path, path_length, colony);

            if (path_length < best_length) {
                best_length = path_length;
                memcpy(best_path, path, best_length * sizeof(int));
            }

            // update global best
            if (path_length < colony->global_best_length) {
                int copy_len = (path_length > colony->global_best_capacity) ? colony->global_best_capacity : path_length;
                memcpy(colony->global_best_path, path, copy_len * sizeof(int));
                colony->global_best_length = path_length;
            }
        }

        // 🔹 Add this line if you want to log every ant’s path:
        log_ant_path(logfile, path, path_length, a);

        free(path);
    }


    // evaporate after all ants
    evaporate_pheromones(g, colony);
    if (best_length < INT_MAX) {
        log_iteration_best(logfile, iteration, g, best_path, best_length, colony);
    }

    free(best_path);
    return 0;
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
    fprintf(csv, "Iteration,BestPathLength,BestPathCost,NormBestCost,GlobalBestLength,GlobalBestCost,ImprovementFactor,Alpha,Beta,Evaporation,Deposit\n");
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

    // 🔹 Append a Final row to CSV
    csv = fopen("convergence.csv", "a");   // ✅ reuse same variable
    if (csv) {
        double global_cost = compute_path_cost(g, colony->global_best_path,
                                            colony->global_best_length);
        double chain_cost = 1.1 * (g->num_nodes - 1);
        double norm = global_cost / chain_cost;
        double improvement = chain_cost / global_cost;

        fprintf(csv, "Final,%d,%.2f,%.4f,%d,%.2f,%.4f,%.2f,%.2f,%.2f,%.2f\n",
                colony->global_best_length,
                global_cost,
                norm,
                colony->global_best_length,
                global_cost,
                improvement,
                colony->alpha,
                colony->beta,
                colony->evaporation_rate,
                colony->deposit_amount);
        fclose(csv);
    }
}