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
            printf("Caminho: ");
            char path[256];
            int path_index = 0;
            for (char v = target_vertex; v != '\0'; v = predecessors[(int)v])
            {
                path[path_index++] = v;
            }
            for (int i = path_index - 1; i >= 0; i--)
            {
                printf("%c", path[i]);
                if (i > 0)
                    printf(" -> ");
            }
            printf("\n");
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