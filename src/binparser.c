#include "binparser.h"

NodeList *parse_data_from_bin_file(const char *filename)
{
    // Tenta abrir o arquivo para leitura em modo binário.
    FILE *bin_file = fopen(filename, "rb");
    if (!bin_file)
    {
        printf("Não foi possível abrir o arquivo: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Aloca uma nova lista em memória.
    NodeList *list = (NodeList *)calloc(1, sizeof(NodeList));
    list->head = NULL;
    list->size = 0;

    // Enquanto não finalizar a leitura
    bool end = false;
    while (!end)
    {
        // Aloca um novo Node em memória
        Node *new_node = (Node *)calloc(1, sizeof(Node));
        if (fread(&new_node->vertex, sizeof(new_node->vertex), 1, bin_file) == 1)
        {
            // Lê os dados do node
            fread(&new_node->x, sizeof(new_node->x), 1, bin_file);
            fread(&new_node->y, sizeof(new_node->y), 1, bin_file);
            fread(new_node->street_1, sizeof(new_node->street_1), 1, bin_file);
            fread(new_node->street_2, sizeof(new_node->street_2), 1, bin_file);

            // Lê as flags que indicam presença de arestas nas direções.
            int north_flag, south_flag, east_flag, west_flag;
            fread(&north_flag, sizeof(int), 1, bin_file);
            fread(&south_flag, sizeof(int), 1, bin_file);
            fread(&east_flag, sizeof(int), 1, bin_file);
            fread(&west_flag, sizeof(int), 1, bin_file);

            // Caso a flag indique a presença, aloca um aresta e salva o ponteiro,
            // caso contrário inicializa o ponteiro como NULL.
            new_node->north = north_flag ? (Edge *)calloc(1, sizeof(Edge)) : NULL;
            new_node->south = south_flag ? (Edge *)calloc(1, sizeof(Edge)) : NULL;
            new_node->east = east_flag ? (Edge *)calloc(1, sizeof(Edge)) : NULL;
            new_node->west = west_flag ? (Edge *)calloc(1, sizeof(Edge)) : NULL;

            // Lê os dados para cara aresta se as flags de presença indicar que
            // tais dados existem.
            if (north_flag)
                fread(new_node->north, sizeof(Edge), 1, bin_file);
            if (south_flag)
                fread(new_node->south, sizeof(Edge), 1, bin_file);
            if (east_flag)
                fread(new_node->east, sizeof(Edge), 1, bin_file);
            if (west_flag)
                fread(new_node->west, sizeof(Edge), 1, bin_file);

            // Adiciona o node no começo da lista.
            new_node->next = list->head;
            list->head = new_node;
            list->size++;
        }
        else
        {
            // Caso chegar ao final do arquivo, libera o node anteriormente
            // alocado e muda a flag de finalização para true.
            free(new_node);
            end = true;
        }
    }

    // Fecha o arquivo de leitura
    fclose(bin_file);

    // Retorna o ponteiro para a lista.
    return list;
}