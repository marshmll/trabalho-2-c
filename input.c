#include "input.h"

void consume_input_garbage(FILE *stream)
{
    char c;
    do
        c = fgetc(stream);
    while (c && c != '\n' && c != EOF);
}

void remove_crlf(char str[])
{
    int CRLF = strcspn(str, "\r\n");
    str[CRLF] = '\0';
}
