#ifndef BINUTIL_H
#define BINUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "input.h"
#include "graph.h"
#include "binutils.h"

NodeList *read_nodes_from_file(FILE *points_file);

void read_edges_from_file(FILE *neighbours_file, NodeList *list);

void write_bin_data(NodeList *list, const char *filename);

#endif /* BINUTIL_H */