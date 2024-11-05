#include "bin_gen.h"

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

    Crossing *crossings_list = read_crossings_from_file(points_file);
    fclose(points_file);

    for (Crossing *debug = crossings_list; debug != NULL; debug = debug->next)
    {
        printf("Ponto %c\n", debug->point.name);
        printf("x: %.2f, y: %.2f\n", debug->point.x, debug->point.y);
        printf("Cruzamento entre %s e %s\n", debug->street_1_name, debug->street_2_name);
    }

    FILE *neighbours_file = fopen(neighbours_fname, "r");

    if (points_file == NULL)
    {
        printf("Não foi possível abrir o arquivo: \"%s\"", neighbours_fname);
        free_crossings(crossings_list);
        exit(1);
    }

    Neighbour *neighbours_list = read_neighbours_from_file(neighbours_file, crossings_list);
    fclose(neighbours_file);

    for (Neighbour *debug = neighbours_list; debug != NULL; debug = debug->next)
    {
        printf("Ponto %c e ponto %c são conectados pela rua %s ", debug->crossing_1->point.name, debug->crossing_2->point.name, debug->street_name);
        printf("e estão à %.2fm de distância.\n", debug->distance);
    }

    free_neighbours(neighbours_list);
    free_crossings(crossings_list);

    return 0;
}

Crossing *read_crossings_from_file(FILE *file)
{
    Crossing *head = NULL;

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

        if (head != NULL)
        {
            crossing->next = head;
            head = crossing;
        }
        else
        {
            head = crossing;
        }
    }

    return head;
}

Neighbour *read_neighbours_from_file(FILE *file, Crossing *crossings_list)
{
    Neighbour *head = NULL;

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
        neighbour->crossing_1 = get_crossing_by_point_name(crossings_list, point_1);
        neighbour->crossing_2 = get_crossing_by_point_name(crossings_list, point_2);

        // Módulo do vetor entre os pontos x e y: sqrt((x2 - x1)² + (y2 - y1)²)
        neighbour->distance = sqrtf(
            powf(neighbour->crossing_2->point.x - neighbour->crossing_1->point.x, 2) +
            powf(neighbour->crossing_2->point.y - neighbour->crossing_1->point.y, 2));

        if (head != NULL)
        {
            neighbour->next = head;
            head = neighbour;
        }
        else
        {
            head = neighbour;
        }
    }

    return head;
}

void free_crossings(Crossing *head)
{
    if (head != NULL)
    {
        free_crossings(head->next);
        free(head);
    }
}

void free_neighbours(Neighbour *head)
{
    if (head != NULL)
    {
        free_neighbours(head->next);
        free(head);
    }
}

Crossing *get_crossing_by_point_name(Crossing *crossing_list, char pname)
{
    Crossing *found = NULL;

    for (Crossing *crossing = crossing_list; crossing != NULL; crossing = crossing->next)
    {
        if (crossing->point.name == pname)
            found = crossing;
    }

    return found;
}
