#include <stdlib.h>
#include "ant_graph.h"

/* Create a new graph with n nodes
* @param n Number of nodes
* @return Pointer 'g' to the created AntGraph
*/
AntGraph* create_ant_graph(int n) {
    AntGraph* g = malloc(sizeof(AntGraph)); // allocate memory for the graph structure
    g->num_nodes = n; // Set the number of nodes
    g->edges = malloc(n * sizeof(double*)); // allocate memory for the adjacency matrix
    for (int i = 0; i < n; i++) { // for each row in the graph
        // allocate 'n' doubles for this row (columns of the matrix)
        // calloc sets all weights to 0.0 initially = no edges yet
        g->edges[i] = calloc(n, sizeof(double));
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
    g->edges[from][to] = weight; // set the weight for the edge from node 'from' to node 'to'
    g->edges[to][from] = weight; // undirected graph so set the reverse edge as well
}