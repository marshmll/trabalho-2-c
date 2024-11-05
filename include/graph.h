#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>

/* TYPEDEFS ======================================================================================= */

typedef struct Node
{
    char vertex;
} Node;

typedef struct Edge
{
    Node *dst;
    float weight;
    struct Edge *next;
} Edge;

typedef struct AdjList
{
    char vertex;
    Edge *head;
    struct AdjList *next;
} AdjList;

typedef struct
{
    AdjList *adj_lists;
} Graph;

/* FUNÇÕES AUXILIARES ============================================================================= */

Node *create_node(char vertex);

Graph *create_graph();

AdjList *add_adj_list(Graph *graph, char vertex);

void add_edge(Graph *graph, char src_vertex, char dst_vertex, float weight);

void free_graph(Graph *graph);

void free_edges(Edge *head);

void free_adj_lists(AdjList *head);

AdjList *get_adj_list_from_vertex(Graph *graph, char vertex);

void find_shortest_path_between(Graph *graph, char start_vertex, char target_vertex);