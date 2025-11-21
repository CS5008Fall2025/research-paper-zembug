#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "aco.h"
#include "ant_graph.h"


/* Choose the next node for an ant using pheromone strength and edge distance
 * @param g The graph
 * @param current The current node of the ant
 * @param visited Array indicating which nodes have been visited
 * @param num_nodes Total number of nodes in the graph
 * @param colony Parameters controlling pheromone vs distance weighting
 */
int pick_next_node(AntGraph* g, int current, int* visited, int num_nodes, AntColony* colony) {
    double total = 0.0; // total appeal across all neighbors (used for probability normalization)
    double appeal[num_nodes]; // array to store appeal values for each neighbor

    // compute appeal for each possible neighbor
    for (int neighbor = 0; neighbor < num_nodes; neighbor++) {
        if (!visited[neighbor] && g->edges[current][neighbor].exists) {
            // pheromone influence: pheromone level raised to colony->pheromone_weight
            double pher = pow(g->edges[current][neighbor].pheromone, colony->pheromone_weight);

            // distance influence: inverse of edge weight raised to colony->distance_weight
            double dist = pow(1.0 / g->edges[current][neighbor].weight, colony->distance_weight);
            appeal[neighbor] = pher * dist; // combined appeal = pheromone influence * distance influence
            total += appeal[neighbor]; // add this appeal to the total (for probability normalization)
        } else {
            appeal[neighbor] = 0.0; // if node already visited or edge doesn’t exist, appeal = 0
        }
    }
    if (total == 0.0) return -1; // if no valid neighbors found, return -1 (dead end)

    // pick a random number between 0 and total appeal
    double r = ((double) rand() / RAND_MAX) * total;

    double cumulative = 0.0; // running sum of appeal values
    for (int neighbor = 0; neighbor < num_nodes; neighbor++) {
        cumulative += appeal[neighbor]; // add this neighbor’s appeal
        if (r <= cumulative) { // if random number falls in this range, choose this neighbor
            return neighbor;
        }
    }
    return -1; // should not reach here
}



/* Construct a path from start to end by repeatedly calling pick_next_node
* @param g The graph
* @param start The starting node
* @param end The ending node
* @param num_nodes Total number of nodes in the graph
* @param path Array to store the constructed path
* @param path_length Pointer to store the length of the constructed path
*/
void build_path(AntGraph* g, int start, int end, int num_nodes, int* path, int* path_length, AntColony* colony) {  
    // allocate array to track visited nodes (all start at 0 = unvisited)
    int* visited = calloc(num_nodes, sizeof(int));
    int current = start; // begin at starting node
    int idx = 0; // index into path array (how many nodes added so far)

    path[idx++] = current; // record starting node in path
    visited[current] = 1; // mark starting node as visited

    // keep moving untiul we reach the end node
    while (current != end) {
        int next = pick_next_node(g, current, visited, num_nodes, colony);// choose next node (pheromone/distance)
        if (next == -1) { // no valid next node found, dead end
            break;
        }
        path[idx++] = next; // add chosen node to path
        visited[next] = 1; // mark chosen node as visited
        current = next; // move forward to chosen node
    }
    *path_length = idx; // record how many nodes are in the path
    free(visited); // free memory used for visited array
}


/* Run the Ant Colony Optimization algorithm
 * @param g The graph
 * @param colony The ant colony (number of ants, evaporation rate, etc.)
 * @param start The starting node
 * @param end The destination node
 * @param iterations Number of iterations to run
 */
void run_aco(AntGraph* g, AntColony* colony, int start, int end, int iterations) {
    printf("Starting ACO with %d ants, %d iterations.\n", colony->num_ants, iterations);

    // main ACO loop for the specified number of iterations
    for (int iteration = 0; iteration < iterations; iteration++) {
        printf("Iteration %d:\n", iteration + 1);
        
        // each ant constructs a path in this iteration
        for (int a = 0; a < colony->num_ants; a++) {
            int path[100];       // array to store the path nodes
            int path_length = 0; // how many nodes are in the path

            // build a path from start to end using pheromone/distance rules
            build_path(g, start, end, g->num_nodes, path, &path_length, colony);

            // print the path taken by this ant
            printf("  Ant %d path:", a + 1);
            for (int i = 0; i < path_length; i++) {
                printf(" %d", path[i]);
            }
            printf("\n");

            // deposit pheromone along the path
            double deposit = (path_length > 0) ? 1.0 / path_length : 0.0;
            for (int i = 0; i < path_length - 1; i++) {
                int u = path[i];
                int v = path[i + 1];
                g->edges[u][v].pheromone += deposit;
                g->edges[v][u].pheromone += deposit; // undirected
            }
        }

        // evaporation step: reduce pheromone on all edges
        for (int i = 0; i < g->num_nodes; i++) {
            for (int j = 0; j < g->num_nodes; j++) {
                if (g->edges[i][j].exists) {
                    g->edges[i][j].pheromone *= (1.0 - colony->evaporation_rate);
                }
            }
        }
        printf("  Pheromones evaporate at rate %.2f.\n", colony->evaporation_rate);

        // Debug: print pheromone matrix after evaporation
        printf("  Pheromone matrix:\n");
        for (int i = 0; i < g->num_nodes; i++) {
            for (int j = 0; j < g->num_nodes; j++) {
                if (g->edges[i][j].exists) {
                    printf("%.2f ", g->edges[i][j].pheromone);
                } else {
                    printf(" .  "); // placeholder for non-existent edge
                }
            }
            printf("\n");
        }
    }

    printf("ACO finished after %d iterations.\n", iterations);
}
