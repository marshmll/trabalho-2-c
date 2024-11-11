#ifndef INPUT_H
#define INPUT_H

#define MAX_STR_LEN 150

#include "graph.h"

/**
 * @brief Consome caracteres de um stream até que encontre
 * um newspace ou EOF.
 *
 * @param stream Um FILE* de onde deverão ser consumudos os
 * caracteres.
 *
 * @return void
 */
void consume_input_garbage(FILE *stream);

/**
 * @brief Substitui quaisquer caracteres de CRLF
 * ('\\r', '\\n') em uma string por NUL ('\\0').
 *
 * @attention A string precisa ser um lvalue modificável.
 * Strings declaradas implicitamente não podem ser passadas
 * como parâmetro.
 *
 * @param str Um array de caracteres.
 *
 * @return void
 */
void remove_crlf(char *str);

#endif /* INPUT_H */