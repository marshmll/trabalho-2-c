#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <string.h>

void consume_input_garbage(FILE *stream);

void remove_crlf(char str[]);

#endif /* INPUT_H */