#ifndef PRINTSTERMINAL_H
#define PRINTSTERMINAL_H

#include "../includes/queries.h"
#include "../includes/prints.h"

void printOneLine_Terminal (char *output);

void printMultipleLines_Terminal (char **output, int lines);

int printQ1F_Terminal(char *output, int output_number, int output_type);

int printQ3F_Terminal(char *output, int output_number);

void printQ4F_Terminal(char **output, int N, int output_number);

void printQ5F_Terminal(char **output, int N, int output_number);

void printQ7F_Terminal(char **output, int N, int output_number);

void printQ8F_Terminal(char *output, int output_number);

void printQ9F_Terminal(char **output, int N, int output_number);

#endif //PRINTSTERMINAL.H