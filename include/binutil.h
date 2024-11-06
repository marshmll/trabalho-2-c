#ifndef BINUTIL_H
#define BINUTIL_H

#define OUTPUT_FNAME "data.bin"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "input.h"
#include "graph.h"

#define MAX_STR_LEN 50


AdjList *read_adj_lists_from_file(FILE *points_file, FILE *neighbours_file);

#endif /* INPUT_H */