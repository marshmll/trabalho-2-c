#ifndef BINPARSER_H
#define BINPARSER_H

#include "graph.h"

/**
 * @brief Recupera uma lista de Nodes de um determinado
 * arquivo binário anteriormente gerado.
 * @attention Recupera todos os nodes e suas arestas. É
 * de responsabilidade do usuário liberar a memória alocada.
 *
 * @param bin_fname O nome do arquivo binário a ser lido.
 *
 * @return NodeList*
 */
NodeList *parse_data_from_bin_file(const char *bin_fname);

#endif /* BINPARSER_H */