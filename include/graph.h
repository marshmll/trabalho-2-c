#include <stdlib.h>

/* TYPEDEFS ======================================================================================= */

typedef struct Node
{
    unsigned int vertex;
} Node;

typedef struct Edge
{
    Node *dst;
    float weight;
    struct Edge *next;
} Edge;

typedef struct AdjList
{
    unsigned int vertex;
    Edge *head;
    struct AdjList *next;
} AdjList;

typedef struct
{
    AdjList *adj_lists;
} Graph;

/* FUNÇÕES AUXILIARES ============================================================================= */

Node *create_node(unsigned int vertex);

Graph *create_graph();

AdjList *add_adj_list(Graph *graph, unsigned int vertex);

void add_edge(Graph *graph, unsigned int src_vertex, unsigned int dst_vertex, float weight);

void free_graph(Graph *graph);

void free_edges(Edge *head);

void free_adj_lists(AdjList *head);

AdjList *get_adj_list_from_vertex(Graph *graph, unsigned int vertex);