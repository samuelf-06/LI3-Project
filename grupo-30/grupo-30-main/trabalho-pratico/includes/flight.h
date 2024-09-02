#ifndef FLIGHT_H
#define FLIGHT_H

#include <glib.h>
#include "../includes/date.h"

typedef struct flight FLIGHT;

typedef struct airport AIRPORTS_FLIGHTS;

typedef struct catalog_flights CATALOG_FLIGHTS;

void delete_Flight (void *data);

FLIGHT *create_Flight (char *line);

void delete_airportFlights (void *data);

CATALOG_FLIGHTS  *init_CatalogFlights();

void insert_Catalog_Flight (CATALOG_FLIGHTS *cf, FLIGHT *f);

void delete_Catalog_Flight(CATALOG_FLIGHTS *cf);

int exist_fligth(CATALOG_FLIGHTS *cf, int id);

int compareFlightsQ5(FLIGHT* f1, FLIGHT* f2);

void add_passenger_to_flight(FLIGHT *f, char *user_id, CATALOG_FLIGHTS *cf);

FLIGHT *get_flight_from_catalog(CATALOG_FLIGHTS *cf, int *flight_id);

GHashTable* get_flights_hash(CATALOG_FLIGHTS *cf);

char* get_airline(FLIGHT *f);

char* get_plane_model(FLIGHT *f);

char* get_origin(FLIGHT *f);

char* get_destination(FLIGHT *f);

DATE_SECS get_schedule_departure_date(FLIGHT *f);

DATE_SECS get_schedule_arrival_date(FLIGHT *f);

int get_numPassengers(FLIGHT *f);

int get_delay(FLIGHT *f);

GList* get_passenger_ids(FLIGHT* flight);

GHashTable* get_airports_flights(CATALOG_FLIGHTS *cf);

int get_flight_id(FLIGHT* flight);

int get_flights_total(AIRPORTS_FLIGHTS *af);

FLIGHT** get_flights(AIRPORTS_FLIGHTS *af);

GArray* get_delays(AIRPORTS_FLIGHTS *airport);

char* get_airport_name(AIRPORTS_FLIGHTS *airport);

#endif //FLIGHT_H
