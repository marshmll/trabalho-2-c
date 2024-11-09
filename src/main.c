#include "graph.h"
#include "binutils.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Modo de uso: %s <dados.bin>\n", argv[0]);
        exit(1);
    }

    Graph *graph = create_graph();
    graph->list = parse_data_from_bin_file(argv[1]);

    char a, b;

    printf("++++++++++++++++++++ NAVEGAÇÃO ++++++++++++++++++++\n\n");
    printf("PONTOS DISPONÍVEIS:\n");
    for (Node *node = graph->list->head; node != NULL; node = node->next)
    {
        printf("    - Ponto %c (%s com %s)\n", node->vertex, node->street_1, node->street_2);
    }
    do
    {

        printf("\n");

        printf("Digite o ponto de partida: ");
        scanf("%c", &a);
        a = toupper(a);
        consume_input_garbage(stdin);

        printf("Digite o ponto de destino: ");
        scanf("%c", &b);
        b = toupper(b);
        consume_input_garbage(stdin);

        printf("\n");

        if (!isalpha(a) || !isalpha(b))
            printf("OS PONTOS DEVEM SER LETRAS. TENTE NOVAMENTE.\n");
        else if (a == b)
            printf("OS PONTOS DEVEM SER DISTINTOS. TENTE NOVAMENTE.\n");
    } while (a == b || !isalpha(a) || !isalpha(b));

    print_shortest_path_between(graph, a, b);
    printf("\n");

    free_graph(graph);
}