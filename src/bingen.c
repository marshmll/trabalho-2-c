#include "bingen.h"

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Modo de uso: %s <pontos.txt> <vizinhos.txt> <saida.bin>\n", argv[0]);
        exit(1);
    }

    char points_fname[MAX_STR_LEN];
    strcpy(points_fname, argv[1]);

    char neighbours_fname[MAX_STR_LEN];
    strcpy(neighbours_fname, argv[2]);

    char bin_fname[MAX_STR_LEN];
    strcpy(bin_fname, argv[3]);

    FILE *points_file = fopen(points_fname, "r");

    if (!points_file)
    {
        printf("Ocorreu um erro ao tentar abrir o arquivo: %s", points_fname);
        exit(EXIT_FAILURE);
    }

    NodeList *node_list = read_nodes_from_file(points_file);

    FILE *neighbours_file = fopen(neighbours_fname, "r");

    if (!neighbours_file)
    {
        printf("Ocorreu um erro ao tentar abrir o arquivo: %s", neighbours_fname);
        free_node_list(node_list);
        exit(EXIT_FAILURE);
    }

    read_edges_from_file(neighbours_file, node_list);

    write_bin_data(node_list, bin_fname);

    free_node_list(node_list);

    fclose(points_file);
    fclose(neighbours_file);

    return 0;
}

NodeList *read_nodes_from_file(FILE *points_file)
{
    NodeList *list = (NodeList *)calloc(1, sizeof(NodeList));
    list->size = 0;

    char buf[MAX_STR_LEN];

    while (fgets(buf, sizeof(buf), points_file))
    {
        Node *node = (Node *)calloc(1, sizeof(Node));
        remove_crlf(buf);

        node->vertex = buf[0];

        fscanf(points_file, "%f%f", &(node->x), &(node->y));
        consume_input_garbage(points_file);

        fgets(node->street_1, sizeof(node->street_1), points_file);
        fgets(node->street_2, sizeof(node->street_2), points_file);
        remove_crlf(node->street_1);
        remove_crlf(node->street_2);

        node->north = NULL;
        node->south = NULL;
        node->east = NULL;
        node->west = NULL;

        if (list->head != NULL)
        {
            node->next = list->head;
            list->head = node;
        }
        else
        {
            list->head = node;
            node->next = NULL;
        }

        list->size++;
    }

    return list;
}

void read_edges_from_file(FILE *neighbours_file, NodeList *list)
{
    char buf[MAX_STR_LEN];

    while (fgets(buf, sizeof(buf), neighbours_file))
    {
        char src_vertex, dst_vertex;
        Node *src_node, *dst_node;

        remove_crlf(buf);
        src_vertex = buf[0];

        fgets(buf, sizeof(buf), neighbours_file);
        remove_crlf(buf);
        dst_vertex = buf[0];

        src_node = get_node_by_vertex(list, src_vertex);
        dst_node = get_node_by_vertex(list, dst_vertex);

        if (src_node != NULL && dst_node != NULL)
        {
            Edge *edge = (Edge *)calloc(1, sizeof(Edge));

            float delta_x = dst_node->x - src_node->x;
            float delta_y = dst_node->y - src_node->y;

            if (delta_x > 0.0)
            {
                strcpy(edge->direction, "Oeste");
                src_node->east = edge;
            }
            else if (delta_x < 0.0)
            {
                strcpy(edge->direction, "Leste");
                src_node->west = edge;
            }
            else if (delta_y > 0.0)
            {
                strcpy(edge->direction, "Sul");
                src_node->south = edge;
            }
            else if (delta_y < 0.0)
            {
                strcpy(edge->direction, "Norte");
                src_node->north = edge;
            }

            edge->dst = dst_vertex;
            edge->weight = sqrtf(pow(delta_x, 2) + pow(delta_y, 2));
            fgets(edge->street, sizeof(edge->street), neighbours_file);
            remove_crlf(edge->street);
        }
    }
}

void write_bin_data(NodeList *list, const char *filename)
{
    FILE *outfile = fopen(filename, "wb");
    if (!outfile)
    {
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
        return;
    }

    for (Node *node = list->head; node != NULL; node = node->next)
    {
        // Write node data
        fwrite(&(node->vertex), sizeof(node->vertex), 1, outfile);
        fwrite(&(node->x), sizeof(node->x), 1, outfile);
        fwrite(&(node->y), sizeof(node->y), 1, outfile);
        fwrite(node->street_1, sizeof(node->street_1), 1, outfile);
        fwrite(node->street_2, sizeof(node->street_2), 1, outfile);

        // Flags to indicate presence of edges
        int north_flag = (node->north != NULL) ? 1 : 0;
        int south_flag = (node->south != NULL) ? 1 : 0;
        int east_flag = (node->east != NULL) ? 1 : 0;
        int west_flag = (node->west != NULL) ? 1 : 0;

        fwrite(&north_flag, sizeof(int), 1, outfile);
        fwrite(&south_flag, sizeof(int), 1, outfile);
        fwrite(&east_flag, sizeof(int), 1, outfile);
        fwrite(&west_flag, sizeof(int), 1, outfile);

        // Write edge data if present
        if (north_flag)
            fwrite(node->north, sizeof(Edge), 1, outfile);
        if (south_flag)
            fwrite(node->south, sizeof(Edge), 1, outfile);
        if (east_flag)
            fwrite(node->east, sizeof(Edge), 1, outfile);
        if (west_flag)
            fwrite(node->west, sizeof(Edge), 1, outfile);
    }

    fclose(outfile);
}