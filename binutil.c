#include "binutil.h"

#define OUTPUT_FNAME "data.bin"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Modo de uso: %s <pontos> <vizinhos>\n", argv[0]);
        exit(1);
    }

    char points_fname[MAX_STR_LEN];
    strcpy(points_fname, argv[1]);

    char neighbours_fname[MAX_STR_LEN];
    strcpy(neighbours_fname, argv[2]);

    FILE *points_file = fopen(points_fname, "r");

    if (points_file == NULL)
    {
        printf("Não foi possível abrir o arquivo: \"%s\"", points_fname);
        exit(1);
    }

    CrossingList *crossing_list = read_crossings_from_file(points_file);
    fclose(points_file);

    for (Crossing *debug = crossing_list->head; debug != NULL; debug = debug->next)
    {
        printf("Ponto %c\n", debug->point.name);
        printf("x: %.2f, y: %.2f\n", debug->point.x, debug->point.y);
        printf("Cruzamento entre %s e %s\n", debug->street_1_name, debug->street_2_name);
    }

    FILE *neighbours_file = fopen(neighbours_fname, "r");

    if (points_file == NULL)
    {
        printf("Não foi possível abrir o arquivo: \"%s\"", neighbours_fname);
        free_crossings_list(crossing_list);
        exit(1);
    }

    NeighbourList *neighbour_list = read_neighbours_from_file(neighbours_file, crossing_list);
    fclose(neighbours_file);

    for (Neighbour *debug = neighbour_list->head; debug != NULL; debug = debug->next)
    {
        printf("Ponto %c e ponto %c são conectados pela rua %s ", debug->crossing_1->point.name, debug->crossing_2->point.name, debug->street_name);
        printf("e estão à %.2fm de distância.\n", debug->distance);
    }

    write_bin_data(OUTPUT_FNAME, crossing_list, neighbour_list);

    free_neighbours_list(neighbour_list);
    free_crossings_list(crossing_list);

    return 0;
}

CrossingList *read_crossings_from_file(FILE *file)
{
    CrossingList *list = (CrossingList *)malloc(sizeof(CrossingList));
    list->head = NULL;
    list->size = 0;

    char buffer[MAX_STR_LEN];

    while (fgets(buffer, MAX_STR_LEN, file))
    {
        char point_name;
        float x;
        float y;
        char street_1_name[MAX_STR_LEN];
        char street_2_name[MAX_STR_LEN];

        remove_crlf(buffer);
        point_name = buffer[0];

        fscanf(file, "%f%f", &x, &y);
        consume_input_garbage(file);

        fgets(street_1_name, MAX_STR_LEN, file);
        remove_crlf(street_1_name);

        fgets(street_2_name, MAX_STR_LEN, file);
        remove_crlf(street_2_name);

        Crossing *crossing = (Crossing *)malloc(sizeof(Crossing));
        crossing->point.x = x;
        crossing->point.y = y;
        crossing->point.name = point_name;
        strcpy(crossing->street_1_name, street_1_name);
        strcpy(crossing->street_2_name, street_2_name);

        crossing->next = list->head;
        list->head = crossing;
        list->size++;
    }

    return list;
}

NeighbourList *read_neighbours_from_file(FILE *file, CrossingList *crossing_list)
{
    NeighbourList *list = (NeighbourList *)malloc(sizeof(NeighbourList));
    list->head = NULL;
    list->size = 0;

    char buffer[MAX_STR_LEN];

    while (fgets(buffer, MAX_STR_LEN, file))
    {
        char point_1;
        char point_2;
        char street_name[MAX_STR_LEN];

        remove_crlf(buffer);

        point_1 = buffer[0];

        fgets(buffer, MAX_STR_LEN, file);
        remove_crlf(buffer);

        point_2 = buffer[0];

        fgets(street_name, MAX_STR_LEN, file);
        remove_crlf(street_name);

        Neighbour *neighbour = (Neighbour *)malloc(sizeof(Neighbour));

        strcpy(neighbour->street_name, street_name);
        neighbour->crossing_1 = get_crossing_by_point_name(crossing_list, point_1);
        neighbour->crossing_2 = get_crossing_by_point_name(crossing_list, point_2);

        // Módulo do vetor entre os pontos x e y: sqrt((x2 - x1)² + (y2 - y1)²)
        neighbour->distance = sqrtf(
            powf(neighbour->crossing_2->point.x - neighbour->crossing_1->point.x, 2) +
            powf(neighbour->crossing_2->point.y - neighbour->crossing_1->point.y, 2));

        neighbour->next = list->head;
        list->head = neighbour;
        list->size++;
    }

    return list;
}

void free_crossings(Crossing *head)
{
    if (head != NULL)
    {
        free_crossings(head->next);
        free(head);
    }
}

void free_crossings_list(CrossingList *list)
{
    free_crossings(list->head);
    free(list);
}

void free_neighbours(Neighbour *head)
{
    if (head != NULL)
    {
        free_neighbours(head->next);
        free(head);
    }
}

void free_neighbours_list(NeighbourList *list)
{
    free_neighbours(list->head);
    free(list);
}

Crossing *get_crossing_by_point_name(CrossingList *crossing_list, char pname)
{
    Crossing *found = NULL;

    for (Crossing *crossing = crossing_list->head; crossing != NULL; crossing = crossing->next)
    {
        if (crossing->point.name == pname)
            found = crossing;
    }

    return found;
}

bool write_bin_data(char *fname, CrossingList *crossing_list, NeighbourList *neighbour_list)
{
    FILE *out_file = fopen(fname, "w+");

    if (out_file == NULL)
        return false;

    const int CROSSINGS_HEADER = 0xC0C0C0C0;
    const int NEIGHBOURS_HEADER = 0xFEFEFEFE;

    fwrite(&CROSSINGS_HEADER, sizeof(CROSSINGS_HEADER), 1, out_file);
    fwrite(&crossing_list->size, sizeof(int), 1, out_file);

    for (Crossing *crossing = crossing_list->head; crossing != NULL; crossing = crossing->next)
    {
        fwrite(&crossing->point.name, sizeof(crossing->point.name), 1, out_file);
        fwrite(&crossing->point.x, sizeof(crossing->point.x), 1, out_file);
        fwrite(&crossing->point.y, sizeof(crossing->point.y), 1, out_file);
        fwrite(crossing->street_1_name, sizeof(crossing->street_1_name), 1, out_file);
        fwrite(crossing->street_2_name, sizeof(crossing->street_2_name), 1, out_file);
    }

    fwrite(&NEIGHBOURS_HEADER, sizeof(NEIGHBOURS_HEADER), 1, out_file);
    fwrite(&neighbour_list->size, sizeof(int), 1, out_file);

    for (Neighbour *neighbour = neighbour_list->head; neighbour != NULL; neighbour = neighbour->next)
    {

        fwrite(&neighbour->crossing_1->point.name, sizeof(neighbour->crossing_1->point.name), 1, out_file);
        fwrite(&neighbour->crossing_2->point.name, sizeof(neighbour->crossing_2->point.name), 1, out_file);
        fwrite(neighbour->street_name, sizeof(neighbour->street_name), 1, out_file);
        fwrite(&neighbour->distance, sizeof(neighbour->distance), 1, out_file);
    }

    fclose(out_file);
    return true;
}
