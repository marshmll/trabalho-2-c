#ifndef BINUTIL_H
#define BINUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "input.h"

#define MAX_STR_LEN 50

typedef struct
{
    char name;
    float x;
    float y;
} Point;

typedef struct Crossing
{
    Point point;
    char street_1_name[MAX_STR_LEN];
    char street_2_name[MAX_STR_LEN];
    struct Crossing *next;
} Crossing;

typedef struct Neighbour
{
    Crossing *crossing_1;
    Crossing *crossing_2;
    char street_name[MAX_STR_LEN];
    float distance;
    struct Neighbour *next;
} Neighbour;

typedef struct
{
    Crossing *head;
    int size;
} CrossingList;

typedef struct
{
    Neighbour *head;
    int size;
} NeighbourList;

CrossingList *read_crossings_from_file(FILE *file);

NeighbourList *read_neighbours_from_file(FILE *file, CrossingList *crossing_list);

void free_crossings(Crossing *head);

void free_crossings_list(CrossingList *list);

void free_neighbours(Neighbour *head);

void free_neighbours_list(NeighbourList *list);

Crossing *get_crossing_by_point_name(CrossingList *crossing_list, char pname);

bool write_bin_data(char *fname, CrossingList *crossing_list, NeighbourList *neighbour_list);

#endif /* INPUT_H */