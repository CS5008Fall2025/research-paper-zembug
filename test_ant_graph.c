#include <stdio.h>
#include <assert.h>
#include "ant_graph.h"

int main() {
    AntGraph* g = create_ant_graph(4); // create a graph with 4 nodes

    // add some edges
    add_edge(g, 0, 1, 2.5);
    add_edge(g, 1, 2, 1.0);
    add_edge(g, 2, 3, 3.2);

    print_ant_graph(g); // print the adjacency matrix

    // verify edges are present
    assert(g->edges[0][1].weight == 2.5);
    assert(g->edges[1][0].weight == 2.5); // undirected
    assert(g->edges[1][2].weight == 1.0);
    assert(g->edges[2][1].weight == 1.0); // undirected
    assert(g->edges[2][3].weight == 3.2);
    assert(g->edges[3][2].weight == 3.2); // undirected

    // verify no edge where none was added
    assert(g->edges[0][2].exists == 0);
    assert(g->edges[0][3].exists == 0);
    assert(g->edges[1][3].exists == 0);
    assert(g->edges[3][1].exists == 0);

    add_edge(g, 5, 1, 2.0); // invalid edge, should print error message
    add_edge(g, -1, 2, 1.5); // negative edge, should print error message

    printf("All tests passed! Even Mordor cannot break this code.\n");

    free_ant_graph(g); // free the graph memory
    return 0;
}