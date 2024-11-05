#include <stdio.h>
#include "graph.h"

int main(void)
{
    Graph *graph = create_graph();

    add_edge(graph, 'A', 'B', 100.0);
    add_edge(graph, 'B', 'C', 50.0);
    add_edge(graph, 'A', 'C', 200.0);
    add_edge(graph, 'C', 'D', 20.0);
    add_edge(graph, 'B', 'D', 40.0);
    add_edge(graph, 'D', 'E', 20.0);
    add_edge(graph, 'B', 'E', 120.0);
    add_edge(graph, 'E', 'A', 300.0);

    find_shortest_path_between(graph, 'B', 'A');

    free_graph(graph);
}