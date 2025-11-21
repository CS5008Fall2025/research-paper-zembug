#include <stdlib.h>
#include <stdio.h>
#include "ant_graph.h"

/* Create a new graph with n nodes
* @param n Number of nodes
* @return Pointer 'g' to the created AntGraph
*/
AntGraph* create_ant_graph(int n) {
    AntGraph* g = malloc(sizeof(AntGraph)); // allocate memory for the graph structure
    g->num_nodes = n; // Set the number of nodes
    
    // allocate memory for n rows (each row is an array of Edge structs)
    g->edges = malloc(n * sizeof(Edge*));
    for (int i = 0; i < n; i++) {
        g->edges[i] = malloc(n * sizeof(Edge)); // allocate n Edge structs for this row
        
        // initialize each edge in the row
        for (int j = 0; j < n; j++) {
            g->edges[i][j].weight = 0.0; // default weight, no edge yet
            g->edges[i][j].pheromone = 1.0; // default pheromone level, baseline
            g->edges[i][j].exists = 0; // edge does not exist
        }
    }
    return g; // Return the created graph
}

/* Free the memory used by the graph
* @param g Pointer to the AntGraph to be freed
*/
void free_ant_graph(AntGraph* g) {
    for (int i = 0; i < g->num_nodes; i++) {
        free(g->edges[i]); // free each row of the adjacency matrix
    }
    free(g->edges); // free the array of row pointers
    free(g); // free the graph structure itself
}

/* Add an edge with a specified weight between two nodes
* @param g Pointer to the AntGraph
* @param from Source node index
* @param to Destination node index
* @param weight Weight of the edge
*/
void add_edge(AntGraph* g, int from, int to, double weight) {
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes) {
        printf("You shall not pass! Invalid edge (%d, %d). Valid range: 0 to %d.\n", from, to, g->num_nodes - 1);
        return;
    }
    // set edge fields for both directions (undirected graph)
    g->edges[from][to].weight = weight; // set the weight
    g->edges[from][to].pheromone = 1.0; // reset pheromone to baseline
    g->edges[from][to].exists = 1; // mark edge as existing

    g->edges[to][from].weight = weight; // set the weight
    g->edges[to][from].pheromone = 1.0; // reset pheromone to baseline
    g->edges[to][from].exists = 1; // mark edge as existing
}

/* Print the adjacency matrix of the graph
 * @param g Pointer to the AntGraph (const since not modifying it)
 */
void print_ant_graph(const AntGraph* g) {
    printf("Adjacency matrix: Ant Graph!\n");

    // loop through each row of the adjacency matrix
    for (int i = 0; i < g->num_nodes; i++) {
        // loop through each column in the current row
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->edges[i][j].exists) {
                // print edge weight and pheromone if edge exists
                printf("%.1f|%.1f ", g->edges[i][j].weight, g->edges[i][j].pheromone);
            } else {
                // print placeholder if no edge
                printf(" . | .  ");
            }
        }
        printf("\n");
    }
}