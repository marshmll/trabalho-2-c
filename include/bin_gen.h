#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char street_name[MAX_STR_LEN];
    Crossing *crossing_1;
    Crossing *crossing_2;
    float distance;
    struct Neighbour *next;
} Neighbour;

Crossing *read_crossings_from_file(FILE *file);

Neighbour *read_neighbours_from_file(FILE *file, Crossing* crossings_list);

void free_crossings(Crossing *head);

void free_neighbours(Neighbour *head);

Crossing *get_crossing_by_point_name(Crossing *crossing_list, char pname);