#include <stdio.h>

#include "graph.h"
#include "binutils.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Modo de uso: %s <dados.bin>\n", argv[0]);
        exit(1);
    }

    Graph *graph = create_graph();
    graph->list = parse_data_from_bin_file(argv[1]);

    find_shortest_path_between(graph, 'A', 'E');

    free_graph(graph);
}