#include <stdio.h>
#include <include/graph.h>

int main(void)
{
    Graph *graph = create_graph();

    add_edge(graph, 0, 1, 100.0);
    add_edge(graph, 1, 0, 100.0);

    free_graph(graph);
}