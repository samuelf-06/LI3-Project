#ifndef QUERIES_H
#define QUERIES_H

typedef struct id_date_type ID_DATE_TYPE;

#include "../includes/flight.h"
#include "../includes/passenger.h"
#include "../includes/reservation.h"
#include "../includes/date.h"

#include <glib.h>

char *Q1_flight(FLIGHT *f);

char *Q1_reservation(RESERVATION *r);

char *Q1_user (USER *u);

typedef struct id_date_type ID_DATE_TYPE;

char *get_id(ID_DATE_TYPE *idt);

DATE get_date(ID_DATE_TYPE *idt);

char *get_type(ID_DATE_TYPE *idt);

GList* get_flights_with_passenger(CATALOG_FLIGHTS* cf, char* passenger_user_id);

GList* get_reservations_with_id(CATALOG_RESERVATIONS* catalog, char* id);

int compare_dates_ID_DATE_TYPE(const void *a, const void *b);

ID_DATE_TYPE** flights_reservations_id_date_type(CATALOG_USERS *cu, CATALOG_FLIGHTS *cf, CATALOG_RESERVATIONS *cr, char *id);

char *average_rate (CATALOG_RESERVATIONS *cr, char *id_hotel);

char **list_by_hotel(CATALOG_RESERVATIONS *cr, char *id, int *N);

FLIGHT** get_all_flights(CATALOG_FLIGHTS* catalog);

char **filterAndSortFlights(CATALOG_FLIGHTS *cf, char *origin, DATE_SECS begin_date, DATE_SECS end_date, int *N);

typedef struct airportyear AirportYear;

int compare_func(gconstpointer a, gconstpointer b);

char **topNAirportsYear(CATALOG_FLIGHTS *catalog, int year, int *N);

typedef struct airportdelays AirportDelays;

gint compare_integers(gconstpointer a, gconstpointer b);

int compare_airport_delays(gconstpointer a, gconstpointer b);

char **topNAirportsMedianDelays(CATALOG_FLIGHTS *cf, int *N);

char *hotel_recipe(CATALOG_RESERVATIONS *cr, char *hotel_id, char *begindate, char *enddate);

int existNum(const char *str);

char **listByPrefix (CATALOG_USERS *cu, char *prefix, int *N);

#endif //QUERIES_H
