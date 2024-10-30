#include <stdio.h>
#include "include/graph.h"

Node *create_node(unsigned int vertex)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->vertex = vertex;
    return node;
}

Graph *create_graph()
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->adj_lists = NULL;

    return graph;
}

AdjList *add_adj_list(Graph *graph, unsigned int vertex)
{
    AdjList *list = (AdjList *)malloc(sizeof(AdjList));
    list->vertex = vertex;
    list->head = NULL;
    list->next = graph->adj_lists;
    graph->adj_lists = list;

    return list;
}

void add_edge(Graph *graph, unsigned int src_vertex, unsigned int dst_vertex, float weight)
{
    Node *node = create_node(dst_vertex);
    Edge *edge = (Edge *)malloc(sizeof(Edge));

    AdjList *src_list = get_adj_list_from_vertex(graph, src_vertex);

    if (src_list == NULL)
        src_list = add_adj_list(graph, src_vertex);

    edge->dst = node;
    edge->weight = weight;
    edge->next = src_list->head;

    src_list->head = edge;
}

void free_graph(Graph *graph)
{
    for (AdjList *list = graph->adj_lists; list != NULL; list = list->next)
    {
        printf("Free-ing egdes from vertex %d\n", list->vertex);
        free_edges(list->head);
    }

    free_adj_lists(graph->adj_lists);
    free(graph);
}

void free_edges(Edge *head)
{
    if (head != NULL)
    {
        printf("%d (weight %.2f) -> ", head->dst->vertex, head->weight);
        free(head->dst);
        free_edges(head->next);
        free(head);
    }
    else
    {
        printf("NULL\n");
    }
}

void free_adj_lists(AdjList *head)
{
    if (head != NULL)
    {
        free_adj_lists(head->next);
        free(head);
    }
}

AdjList *get_adj_list_from_vertex(Graph *graph, unsigned int vertex)
{
    AdjList *found_list = NULL;

    for (AdjList *list = graph->adj_lists; list != NULL; list = list->next)
        if (list->vertex == vertex)
            found_list = list;

    return found_list;
}
