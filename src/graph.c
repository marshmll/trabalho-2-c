#include <stdio.h>
#include "graph.h"

Graph *create_graph()
{
    Graph *graph = (Graph *)calloc(1, sizeof(Graph));
    graph->list = NULL;

    return graph;
}

Node *get_node_by_vertex(NodeList *list, char vertex)
{
    Node *found = NULL;

    for (Node *node = list->head; node != NULL; node = node->next)
        if (node->vertex == vertex)
            found = node;

    return found;
}

Edge *get_edge_by_dst_vertex(Node *node, char vertex)
{
    if (node->north != NULL && node->north->dst == vertex)
        return node->north;

    if (node->south != NULL && node->south->dst == vertex)
        return node->south;

    if (node->east != NULL && node->east->dst == vertex)
        return node->east;

    if (node->west != NULL && node->west->dst == vertex)
        return node->west;

    return NULL;
}

char *get_instruction_from_orientation_diff(char *orientation_1, char *orientation_2)
{
    char *direction = (char *)calloc(MAX_STR_LEN, sizeof(char));

    if (strcmp(orientation_1, orientation_2) == 0)
        strcpy(direction, "mantenha-se na mesma direção.");
    else
    {
        if (strcmp(orientation_1, "Norte") == 0 && strcmp(orientation_2, "Leste") == 0)
            strcpy(direction, "vire à direita.");
        else if (strcmp(orientation_1, "Leste") == 0 && strcmp(orientation_2, "Sul") == 0)
            strcpy(direction, "vire à direita.");
        else if (strcmp(orientation_1, "Sul") == 0 && strcmp(orientation_2, "Oeste") == 0)
            strcpy(direction, "vire à direita.");
        else if (strcmp(orientation_1, "Oeste") == 0 && strcmp(orientation_2, "Norte") == 0)
            strcpy(direction, "vire à direita.");

        if (strcmp(orientation_1, "Norte") == 0 && strcmp(orientation_2, "Oeste") == 0)
            strcpy(direction, "vire à esquerda.");
        else if (strcmp(orientation_1, "Oeste") == 0 && strcmp(orientation_2, "Sul") == 0)
            strcpy(direction, "vire à esquerda.");
        else if (strcmp(orientation_1, "Sul") == 0 && strcmp(orientation_2, "Leste") == 0)
            strcpy(direction, "vire à esquerda.");
        else if (strcmp(orientation_1, "Leste") == 0 && strcmp(orientation_2, "Norte") == 0)
            strcpy(direction, "vire à esquerda.");
    }

    return direction;
}

void free_graph(Graph *graph)
{
    free_node_list(graph->list);
    free(graph);
}

void free_node_list(NodeList *list)
{
    free_nodes(list->head);
    free(list);
}

void free_nodes(Node *head)
{
    if (head != NULL)
    {
        free_nodes(head->next);

        if (head->north != NULL)
            free(head->north);

        if (head->south != NULL)
            free(head->south);

        if (head->east != NULL)
            free(head->east);

        if (head->west != NULL)
            free(head->west);

        free(head);
    }
}

void find_shortest_path_between(Graph *graph, char start_vertex, char target_vertex)
{
    // Vetor para armazenar custos de salto
    float costs[256];
    for (int i = 0; i < 256; ++i)
        costs[i] = FLT_MAX;

    // Vetor para armazenar quais vértices foram visitados
    bool visited[256] = {false};

    // Vetor para armazenar quais são os vértices predecessores.
    char predecessors[256];
    for (int i = 0; i < 256; ++i)
        predecessors[i] = '\0';

    // Inicializa o vértice de partida como custo 0.
    costs[(int)start_vertex] = 0;

    // Vetor para armazenar a fila de prioridade.
    char priority_queue[256];
    int queue_size = 0;

    // Insere o vértice de partida na fila de prioridade.
    priority_queue[queue_size++] = start_vertex;

    // Enquanto a fila não estiver vazia e não for encontrado o vértice alvo
    bool done = false;
    while (queue_size != 0 && !done)
    {
        // Extrai o vértice com menor custo da fila.
        float min_cost = FLT_MAX;
        int min_index = -1;

        for (int i = 0; i < queue_size; i++)
        {
            char vertex = priority_queue[i];
            if (costs[(int)vertex] < min_cost && !visited[(int)vertex])
            {
                min_cost = costs[(int)vertex];
                min_index = i;
            }
        }

        // Se não encontrou nenhum vértice válido, sair do loop
        if (min_index == -1)
            break;

        // Remove o vértice com o menor custo
        char current_vertex = priority_queue[min_index];
        priority_queue[min_index] = priority_queue[--queue_size];
        visited[(int)current_vertex] = true;

        if (current_vertex == target_vertex)
        {
            printf("Menor distância entre %c e %c: %.2f\n", start_vertex, target_vertex, costs[(int)current_vertex]);
            printf("Caminho:\n");

            // Armazena o caminho em ordem inversa
            Node *path[256];
            int path_size = 0;
            for (char v = target_vertex; v != '\0'; v = predecessors[(int)v])
                path[path_size++] = get_node_by_vertex(graph->list, v);

            // Percorre o caminho para gerar instruções
            int counter = 1;
            for (int i = path_size - 2; i >= 0; i--)
            {
                Node *current_node = path[i + 1];
                Node *next_node = path[i];
                Edge *current_edge = get_edge_by_dst_vertex(current_node, next_node->vertex);
                Edge *next_edge = (i > 0) ? get_edge_by_dst_vertex(next_node, path[i - 1]->vertex) : NULL;

                char *instruction = (next_edge != NULL)
                                        ? get_instruction_from_orientation_diff(current_edge->direction, next_edge->direction)
                                        : NULL;

                // Determina a rua adjacente
                char *adjacent_street = (strcmp(next_node->street_1, current_edge->street) != 0)
                                            ? next_node->street_1
                                            : next_node->street_2;

                // Imprime instruções de acordo com a existência de uma próxima aresta
                printf("    (%d) Siga em frente pela %s até o cruzamento com a %s\n", counter, current_edge->street, adjacent_street);
                counter++;

                if (next_edge != NULL)
                {
                    printf("    (%d) No cruzamento da %s com a %s, %s\n", counter, current_edge->street, adjacent_street, instruction);
                    counter++;
                    free(instruction);
                }
                else
                {
                    printf("Você chegou ao seu destino!\n");
                }
            }

            done = true;
        }

        else
        {
            // Atualiza os custos para os vértices adjacentes.
            Node *current_node = get_node_by_vertex(graph->list, current_vertex);

            if (current_node != NULL)
            {
                Edge *current_node_edges[] = {current_node->north, current_node->south, current_node->east, current_node->west};
                for (int i = 0; i < 4; i++)
                {
                    Edge *edge = current_node_edges[i];

                    if (edge != NULL)
                    {
                        char neighbor = edge->dst;

                        // Calcula o novo custo como sendo o custo do vértice atual mais o
                        // custo do vértice adjacente
                        float new_cost = costs[(int)current_vertex] + edge->weight;

                        // Se o custo calculado é menor do que o custo anterior do vértice adjacente
                        if (new_cost < costs[(int)neighbor])
                        {
                            // Atualiza o custo para o menor e adiciona ao predecessores.
                            costs[(int)neighbor] = new_cost;
                            predecessors[(int)neighbor] = current_vertex;

                            // Procura se o vértice adjacente já está na fila de prioridade
                            bool found = false;
                            for (int i = 0; i < queue_size; i++)
                            {
                                if (priority_queue[i] == neighbor)
                                    found = true;
                            }

                            // Se não, adiciona-o.
                            if (!found)
                                priority_queue[queue_size++] = neighbor;
                        }
                    }
                }
            }
        }
    }
    if (!done)
        printf("Não há caminho de %c à %c.\n", start_vertex, target_vertex);
}