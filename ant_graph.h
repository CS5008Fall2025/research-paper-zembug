#ifndef ANT_GRAPH_H
#define ANT_GRAPH_H

// Structure representing a single edge in the graph
typedef struct Edge {
    double weight;      // Weight (distance) of the edge
    double pheromone;   // Pheromone level on the edge
    int exists;         // Boolean flag indicating if the edge exists (1 = yes, 0 = no)
} Edge;


// Structure representing a graph for the Ant Colony Optimization algorithm
typedef struct AntGraph {
    int num_nodes; // Number of nodes in the graph
    Edge** edges; // Adjacency matrix representing edges
} AntGraph;


// Create a new graph with n nodes
AntGraph* create_ant_graph(int n);

// Free the memory used by the graph
void free_ant_graph(AntGraph* graph);

// Add an edge with a specified weight between two nodes
void add_edge(AntGraph* graph, int from, int to, double weight);

// Print the adjacency matrix of the graph
void print_ant_graph(const AntGraph* g);


#endif