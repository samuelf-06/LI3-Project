#ifndef PRINTS_H
#define PRINTS_H

#include "../includes/queries.h"

const char *getUserFieldName(int Index);
const char *getFlightFieldName(int Index);
const char *getReservationFieldName(int Index);
const char *getQ4FieldName(int fieldIndex);
const char *getQ5FieldName(int fieldIndex);
const char *getQ7FieldName(int fieldIndex);
const char *getQ9FieldName(int fieldIndex);

int printOneLine(char *output, int output_number);

int printMultipleLines(char **output, int N, int output_number);

int printQ1F(char *output, int output_number, int output_type);

int printQ2(ID_DATE_TYPE** id_date_type_array, int output_number, char *type);

int printQ2F(ID_DATE_TYPE** id_date_type_array, int output_number, char *type);

int printQ3F (char *output, int output_number);

int printQ4F(char **output, int N, int output_number);

int printQ5F(char **output, int N, int output_number);

int printQ7F(char **output, int N, int output_number);

int printQ8F (char *output, int output_number);

int printQ9F(char **output, int N, int output_number);

void print_errors(const char *filename, const char *line);

#endif //PRINTS_H
