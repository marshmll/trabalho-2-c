#include "graph.h"
#include "binparser.h"

int main(int argc, char **argv)
{
    // Verifica se o uso do programa está correto.
    if (argc != 2)
    {
        fprintf(stderr, "Modo de uso: %s <dados.bin>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Aloca um novo grafo.
    Graph *graph = create_graph();

    // Lê a lista de nós do grafo do arquivo binário fornecido.
    graph->list = parse_data_from_bin_file(argv[1]);

    // Armazena dois pontos dados pelo usuário.
    char a, b;

    // Imprime um cabeçalho com todas os pontos que podem ser escolhidos
    // pelo usuário.
    printf("++++++++++++++++++++ NAVEGAÇÃO ++++++++++++++++++++\n\n");
    printf("PONTOS DISPONÍVEIS:\n");
    for (Node *node = graph->list->head; node != NULL; node = node->next)
    {
        printf("> Ponto %c (%s com %s)\n", node->vertex, node->street_1, node->street_2);
    }

    // Coleta dois pontos do usuário enquanto forem iguais ou não
    // forem letras.
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

    // Imprime as instruções para se chegar de a À b pelo menor caminho,
    // se existir.
    print_shortest_path_between(graph, a, b);
    printf("\n");

    // Libera toda memória alocada pelo grafo.
    free_graph(graph);

    return 0;
}