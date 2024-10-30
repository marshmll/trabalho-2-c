#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Modo de uso: %s <pontos> <vizinhos>\n", argv[0]);
        exit(1);
    }
}