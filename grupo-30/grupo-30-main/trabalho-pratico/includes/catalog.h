#ifndef CATALOG_H
#define CATALOG_H

#include "../includes/queries.h"

typedef struct catalog CATALOG;

CATALOG *create_Catalog(char *entry_files);

void delete_Catalog(CATALOG *c);

char *Q1(CATALOG *c, char *id);

int auxQ1(char *id);

ID_DATE_TYPE** Q2(CATALOG *c, char *id);

char *Q3 (char *id_hotel, CATALOG *c);

char **Q4 (CATALOG *c, char *id, int *N);

char **Q5 (CATALOG *c, char* origin, char *begindate, char *enddate, int *N);

char **Q6 (CATALOG *c, char *year, int *N);

char **Q7 (CATALOG *c, int *N);

char *Q8(CATALOG *c, char *hotel_id, char *begindate, char *enddate);

char **Q9 (CATALOG *c, char *prefix, int *N);

void parseQueries(char *line, CATALOG *c, int output_number);

int run_batch(char **efile);

int run_interactiveMode();

#endif //CATALOG_H
