#include "binutils.h"

NodeList *parse_data_from_bin_file(const char *filename)
{
    FILE *bin_file = fopen(filename, "rb");
    if (!bin_file)
    {
        perror("Failed to open binary file");
        return NULL;
    }

    NodeList *list = (NodeList *)calloc(1, sizeof(NodeList));
    list->head = NULL;
    list->size = 0;

    bool end = false;
    while (!end)
    {
        Node *new_node = (Node *)calloc(1, sizeof(Node));
        if (fread(&new_node->vertex, sizeof(new_node->vertex), 1, bin_file) == 1)
        {
            // Read node data
            fread(&new_node->x, sizeof(new_node->x), 1, bin_file);
            fread(&new_node->y, sizeof(new_node->y), 1, bin_file);
            fread(new_node->street_1, sizeof(new_node->street_1), 1, bin_file);
            fread(new_node->street_2, sizeof(new_node->street_2), 1, bin_file);

            // Read edge presence flags
            int north_flag, south_flag, east_flag, west_flag;
            fread(&north_flag, sizeof(int), 1, bin_file);
            fread(&south_flag, sizeof(int), 1, bin_file);
            fread(&east_flag, sizeof(int), 1, bin_file);
            fread(&west_flag, sizeof(int), 1, bin_file);

            // Initialize pointers
            new_node->north = north_flag ? (Edge *)calloc(1, sizeof(Edge)) : NULL;
            new_node->south = south_flag ? (Edge *)calloc(1, sizeof(Edge)) : NULL;
            new_node->east = east_flag ? (Edge *)calloc(1, sizeof(Edge)) : NULL;
            new_node->west = west_flag ? (Edge *)calloc(1, sizeof(Edge)) : NULL;

            // Read edge data if present
            if (north_flag)
                fread(new_node->north, sizeof(Edge), 1, bin_file);
            if (south_flag)
                fread(new_node->south, sizeof(Edge), 1, bin_file);
            if (east_flag)
                fread(new_node->east, sizeof(Edge), 1, bin_file);
            if (west_flag)
                fread(new_node->west, sizeof(Edge), 1, bin_file);

            // Add node to the list
            new_node->next = list->head;
            list->head = new_node;
            list->size++;
        }
        else
            end = true;
    }

    fclose(bin_file);
    return list;
}