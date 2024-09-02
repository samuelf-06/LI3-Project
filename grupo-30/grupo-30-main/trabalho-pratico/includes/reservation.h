#ifndef RESERVATION_H
#define RESERVATION_H

#include <glib.h>

#include "../includes/user.h"
#include "../includes/date.h"

typedef struct reservation RESERVATION;

typedef struct hotel_reservations HOTEL_RESERVATIONS;

typedef struct catalog_reservations CATALOG_RESERVATIONS;

void delete_Reservation (void *data);

RESERVATION *create_Reservation (char *line, CATALOG_USERS *cu);

void delete_hotelReservations (void *data);

CATALOG_RESERVATIONS *init_CatalogReservations();

void delete_Catalog_Reservation (CATALOG_RESERVATIONS *cr);

int get_number_nights (RESERVATION *r);

double total_cost_reservation (RESERVATION *r);

void insert_Reservation(CATALOG_RESERVATIONS *cr, RESERVATION *r, CATALOG_USERS *cu);

int compareReservations(const void *a, const void *b);

GHashTable* get_reservations_hash(CATALOG_RESERVATIONS *cr);

char* get_reservation_id(RESERVATION *r);

char *get_hotel_id(RESERVATION *r);

char *get_hotel_name(RESERVATION *r);

int get_hotel_stars(RESERVATION *r);

DATE get_begin_date(RESERVATION *r);

DATE get_end_date(RESERVATION *r);

char *get_includes_breakfast(RESERVATION *r);

int get_nights(RESERVATION *r);

double get_total_cost(RESERVATION *r);

char* get_user_id_reservation(RESERVATION* res);

GHashTable* get_hotel_reservations(CATALOG_RESERVATIONS* cr);

int get_rates_num(HOTEL_RESERVATIONS *hr);

int get_rates_sum(HOTEL_RESERVATIONS* hr);

int get_reservation_rating(RESERVATION *r);

int get_reservations_total(HOTEL_RESERVATIONS *hr);

RESERVATION** get_reservations(HOTEL_RESERVATIONS *hr);

int get_price_per_night(RESERVATION *reservation);

#endif //RESERVATION_H
