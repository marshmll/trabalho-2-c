#include "binutil.h"

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

    return 0;
}