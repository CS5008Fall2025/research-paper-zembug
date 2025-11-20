#ifndef ANT_GRAPH_H
#define ANT_GRAPH_H

typedef struct {
    int num_nodes; // Number of nodes in the graph
    double** edges; // Adjacency matrix representing edge weights
} AntGraph;


// Create a new graph with n nodes
AntGraph* create_ant_graph(int n);

// Free the memory used by the graph
void free_ant_graph(AntGraph* graph);

// Add an edge with a specified weight between two nodes
void add_edge(AntGraph* graph, int from, int to, double weight);


#endif