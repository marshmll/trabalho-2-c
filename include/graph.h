#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>

/* TYPEDEFS ======================================================================================= */

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
    Node *next;
} Node;

typedef struct Edge
{
    Node *dst;
    float weight;
    char street[MAX_STR_LEN];
} Edge;

typedef struct
{
    Node *head;
    Node *tail;
} Graph;

/* FUNÇÕES AUXILIARES ============================================================================= */

void find_shortest_path_between(Graph *graph, char start_vertex, char target_vertex);

#endif