#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>

#include "input.h"

/* TYPEDEFS ======================================================================================= */

#pragma pack(1)
typedef struct Edge
{
    char dst;
    float weight;
    char direction[MAX_STR_LEN];
    char street[MAX_STR_LEN];
} Edge;

typedef struct Node
{
    char vertex;
    float x;
    float y;
    char street_1[MAX_STR_LEN];
    char street_2[MAX_STR_LEN];
    Edge *north;
    Edge *south;
    Edge *east;
    Edge *west;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
    int size;
} NodeList;

typedef struct
{
    NodeList *list;
} Graph;
#pragma pack()

typedef struct PathNode
{
    char src, dst;
    char street_1[MAX_STR_LEN];
    char street_2[MAX_STR_LEN];
    char conn_street[MAX_STR_LEN];
    char direction[MAX_STR_LEN];
    struct PathNode *next;
} PathNode;

/* FUNÇÕES AUXILIARES ============================================================================= */

Graph *create_graph();

Node *get_node_by_vertex(NodeList *list, char vertex);

Edge *get_edge_by_dst_vertex(Node *node, char vertex);

char *get_instruction_from_orientation_diff(char *orientation_1, char *orientation_2);

void free_graph(Graph *graph);

void free_node_list(NodeList *list);

void free_nodes(Node *head);

void find_shortest_path_between(Graph *graph, char start_vertex, char target_vertex);

#endif