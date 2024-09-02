#ifndef PASSENGER_H
#define PASSENGER_H

#include "../includes/user.h"
#include "../includes/flight.h"

typedef struct passenger PASSENGER;

typedef struct catalog_passenger CATALOG_PASSENGERS;

PASSENGER *create_Passenger(char *line, CATALOG_USERS *cu, CATALOG_FLIGHTS *cf);

void delete_Passenger(void *data);

CATALOG_PASSENGERS *init_CatalogPassengers();

void insert_Catalog_Passenger(CATALOG_PASSENGERS *cp, PASSENGER *p, CATALOG_USERS *cu, CATALOG_FLIGHTS *cf);

void delete_Catalog_Passenger(CATALOG_PASSENGERS *cp);

int exist_Passenger(CATALOG_PASSENGERS *cp, char* user_id);

GHashTable* get_passengers_hash(CATALOG_PASSENGERS *cp);

#endif //PASSENGER_H