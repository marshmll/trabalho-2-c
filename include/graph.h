#ifndef GRAPH_H
#define GRAPH_H

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

#define MAX_ASCII_CHARS 256

/* TYPEDEFS ======================================================================================= */

/**
 * @struct Edge
 * @brief Representação de uma aresta de um Grafo.
 *
 * @param dst O vértice de destino da aresta.
 * @param weight O custo da aresta.
 * @param direction A direção para a qual a aresta está orientada.
 * @param street O nome da rua pelo qual a aresta perpassa.
 */
typedef struct Edge
{
    char dst;
    float weight;
    char direction[MAX_STR_LEN];
    char street[MAX_STR_LEN];
} Edge;

/**
 * @struct Node
 * @brief Representação de um nó de um Grafo. Usado para representar
 * cruzamentos entre duas ruas.
 *
 * @param vertex O nome do vértice o qual o nó representa.
 * @param x A coordenada x do nó.
 * @param y A coordenada y do nó.
 * @param street_1 O nome da primeira rua componente do cruzamento.
 * @param street_2 O nome da segunda rua componente do cruzamento.
 * @param north Um ponteiro para a aresta que conecta o nó a outro nó na direção norte.
 * @param south Um ponteiro para a aresta que conecta o nó a outro nó na direção sul.
 * @param east Um ponteiro para a aresta que conecta o nó a outro nó na direção leste.
 * @param west Um ponteiro para a aresta que conecta o nó a outro nó na direção oeste.
 * @param next Um ponteiro para o próximo nó, quando em uma lista.
 */
typedef struct Node
{
    char vertex;
    float x;
    float y;
    char street_1[MAX_STR_LEN];
    char street_2[MAX_STR_LEN];
    Edge *north;
    Edge *south;
    Edge *east;
    Edge *west;
    struct Node *next;
} Node;

/**
 * @struct NodeList
 * @brief Representação de uma lista de nós (Nodes).
 *
 * @param head O ponteiro para o primeiro node da lista.
 * @param size A quantidade de nodes presentes na lista.
 */
typedef struct
{
    Node *head;
    int size;
} NodeList;

/**
 * @struct Graph
 * @brief Representação de um Grafo.
 *
 * @param list Uma lista de Nodes que possuem arestas entre si.
 */
typedef struct
{
    NodeList *list;
} Graph;

/* FUNÇÕES AUXILIARES ============================================================================= */

/**
 * @brief Aloca um grafo (Graph) dinamicamente.
 * @attention Inicializa a lista de Nodes como null.
 *
 * @return Graph *
 */
Graph *create_graph();

/**
 * @brief Procura e retorna um Node contido em uma lista
 * cujo vértice seja igual ao vértice dado, ou NULL se não
 * encontrado.
 *
 * @param list Uma lista de nós (NodeList *)
 * @param vertex O vértice (um caracter) do nó desejado.
 *
 * @return Node *
 */
Node *get_node_by_vertex(NodeList *list, char vertex);

/**
 * @brief Procura e retorna a aresta contida em um Node cujo
 * vértice de destino seja igual ao vértice dado, ou NULL se
 * não encontrado.
 *
 * @param node O nó o qual contém (ou não) a aresta.
 * @param vertex O vértice de destino desejado para a aresta.
 *
 * @return Edge *
 */
Edge *get_edge_by_dst_vertex(Node *node, char vertex);

/**
 * @brief Retorna a instrução de navegação apropriada a partir
 * de duas orientações geográficas.
 * @attention Retorna "vire à direita" caso a variação seja no sentido horário;
 * @attention Retorna "vire à esquerda" caso a variação seja no sentido anti-horário;
 * @attention Retorna "mantenha-se em frente" caso não haja variação no sentido.
 *
 * @return char * (alocado dinamicamente, DEVE SER LIBERADO APÓS O USO)
 */
char *get_instruction_from_orientation_diff(char *orientation_1, char *orientation_2);

/**
 * @brief Libera toda a memória alocada de um grafo (Graph).
 *
 * @param graph Um ponteiro para um grafo.
 *
 * @return void.
 */
void free_graph(Graph *graph);

/**
 * @brief Libera toda a memória alocada de uma lista de nós (NodeList).
 *
 * @param list Um ponteiro para uma lista de nós.
 *
 * @return void
 */
void free_node_list(NodeList *list);

/**
 * @brief Libera toda a memória alocada de nós encadeados em uma lista ligada.
 *
 * @param head Um ponteiro para o primeiro nó da lista.
 *
 * @return void
 */
void free_nodes(Node *head);

/**
 * @brief Imprime na tela uma sequência de instruções para se chegar em um vértice
 * alvo (destino) partindo de um vértice de início (origem).
 * @attention Utiliza o Algoritmo de Dijkstra para determinar o melhor caminho.
 * @attention Utiliza as arestas disponíveis em cada nó, bem como seus pesos para
 * determinar o melhor caminho.
 *
 * @param graph Um ponteiro para um grafo.
 * @param start_vertex O vértice de partida.
 * @param target_vertex O vértice alvo.
 *
 * @return void
 */
void print_shortest_path_between(Graph *graph, char start_vertex, char target_vertex);

#endif