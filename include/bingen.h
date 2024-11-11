#ifndef BINUTIL_H
#define BINUTIL_H

#include "input.h"
#include "graph.h"

/**
 * @brief Lê todos os nodes de um dado arquivo de pontos e
 * os armazena em um NodeList.
 * @attention Todas as arestas de todos os nodes são inicializadas
 * como NULL. O usuário devve liberar a memória ao final do uso.
 *
 * @param points_file Um FILE* para o arquivo de pontos.
 *
 * @return NodeList *
 */
NodeList *read_nodes_from_file(FILE *points_file);

/**
 * @brief Lê todas as arestas presentes em um arquivo de vizinhos
 * em um NodeList*.
 * @attention As direções das arestas são calculadas com base nas
 * coordenadas dos nodes. As direções podem ser: Norte, Sul, Leste
 * ou Oeste.
 *
 * @param neighbours_file Um FILE* para o arquivo de vizinhos.
 * @param list Um NodeList* para os nodes anteriormente lidos.
 *
 * @return void
 */
void read_edges_from_file(FILE *neighbours_file, NodeList *list);

/**
 * @brief Escreve um arquivo binário (.bin) contendo todos os nodes e
 * arestas de uma lista de nodes.
 * @attention Primeiramente o node na ordem de leitura é escrito. Em
 * seguida, são escritas 4 flags indicando a presença ou não de uma
 * aresta em uma determinada direção. Com base nas flags, é possível
 * recuperar posteriormente os dados lendo apenas as direções as quais
 * existe uma flag positiva.
 *
 * @param list Uma lista de nodes a serem escritos no arquivo.
 * @param filename O nome do arquivo gerado.
 *
 * @return true se bem-sucedido, senão false.
 */
bool write_bin_data(NodeList *list, const char *filename);

#endif /* BINUTIL_H */