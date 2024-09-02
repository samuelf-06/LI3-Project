#include "../includes/reservation.h"
#include "../includes/date.h"
#include "../includes/validator.h"
#include "../includes/prints.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>
#include <stdio.h>


typedef struct reservation{
    char *id;
    char *user_id;
    char *hotel_id;
    char *hotel_name;
    int hotel_stars;
    int city_tax;
    char *address;
    DATE begin_date;
    DATE end_date;
    int price_per_night;
    char *includes_breakfast; //true or false
    char *room_details;
    int rating;
    char *comment;

    double total_cost;
    int nights;
} RESERVATION;

typedef struct hotel_reservations {
    int rates_sum, rates_num;
    int reservations_total;
    int size;
    RESERVATION **reservations;
    GList *hotels_sorted;
} HOTEL_RESERVATIONS;

typedef struct catalog_reservations {
    GHashTable *reservations_hash;
    GHashTable *hotel_reservations;
}CATALOG_RESERVATIONS;


// Apaga uma reserva libertando a memória alocada dos seus componentes.
void delete_Reservation (void *data) {
    RESERVATION* r = (RESERVATION *) data;
    free(r->id);
    free(r->user_id);
    free(r->hotel_id);
    free(r->hotel_name);
    free(r->address);
    free(r->room_details);
    free(r->comment);
    free(r);
}

// Cria, alocando memória, uma estrutura reserva (RESERVATION) e garante a validade dos seus dados.
RESERVATION *create_Reservation (char *line, CATALOG_USERS *cu) {
    RESERVATION *r = malloc (sizeof (struct reservation));
    char *token;
    int i = 0, verify = 1;
    char *copy = strdup(line);
    while ((token = strsep(&line, ";")) != NULL)
    {
        switch (i++)
        {
            case 0:
                if (strlen(token) == 0) verify = 0;
                r->id = strdup(token);
                break;
            case 1:
                if (strlen(token) == 0) verify = 0;
                r->user_id = strdup(token);
                if (!exist_user(cu, r->user_id))
                verify = 0;
                break;
            case 2:
                if (strlen(token) == 0) verify = 0;
                r->hotel_id = strdup(token);
                break;
            case 3:
                if (strlen(token) == 0) verify = 0;
                r->hotel_name = strdup(token);
                break;
            case 4:
                if (verify_stars(token) == 0) verify = 0;
                if (isInteger(token) == 0) verify = 0;
                r->hotel_stars = (int) strtol(token, (char**) NULL, 10);
                if (r->hotel_stars < 0)
                    verify = 0;
                break;
            case 5:
                if (isInteger(token) == 0) verify = 0;
                r->city_tax = (int) strtol(token, (char**) NULL, 10);
                if (r->city_tax < 0)
                    verify = 0;
                break;
            case 6:
                if (strlen(token) == 0) verify = 0;
                r->address = strdup(token);
                break;
            case 7:
                if (verify_date(token) == 0)
                    verify = 0;
                r->begin_date = convertStringToDate(token);
                break;
            case 8:
                if (verify_date(token) == 0)
                    verify = 0;
                r->end_date = convertStringToDate(token);
                break;
            case 9:
                if (verify_price(token) == -1)
                    verify = 0;
                r->price_per_night = (int) strtol(token, (char**) NULL, 10);
                break;
            case 10:
                if (verify_breakfast(token) == NULL)
                    verify = 0;
                r->includes_breakfast = verify_breakfast(token);
                break;
            case 11:
                r->room_details = strdup(token);
                break;
            case 12:
                r->rating = verify_rate(token);
                break;

            case 13:
                r->comment = strdup(token);
                break;

        }
    }
    if(verify != 0){
        if (days_from_epoch(r->end_date) - days_from_epoch(r->begin_date) < 0)
            verify = 0;
    }
    if (verify == 0)
    {
        print_errors("Resultados/reservations_errors.csv",copy);
        free(copy);
        delete_Reservation(r);
        return NULL;
    }
    r->total_cost = 0;
    r->nights = 0;
    free(copy);
    return r;
}

// Apaga uma reserva de hotel libertando a memória alocada dos seus componentes e o próprio.
void delete_hotelReservations (void *data) {
    HOTEL_RESERVATIONS *hr = (HOTEL_RESERVATIONS *) data;
    g_list_free(hr->hotels_sorted);
    free(hr->reservations);
    free(hr);
}

// Auxiliar para inicializar, alocando memória, o catálogo (Hashtable) das reservas.
CATALOG_RESERVATIONS *init_CatalogReservations() {
    CATALOG_RESERVATIONS *cr = malloc(sizeof (CATALOG_RESERVATIONS));
    cr->hotel_reservations = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, delete_hotelReservations);
    cr->reservations_hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, delete_Reservation);
    return cr;
}

// Apaga um catálogo de reservas libertando a memória alocada dos seus componentes e do próprio.
void delete_Catalog_Reservation(CATALOG_RESERVATIONS *cr) {
    g_hash_table_destroy(cr->reservations_hash);
    g_hash_table_destroy(cr->hotel_reservations);
    free(cr);
}

// Calcula o número de noites de uma reserva.
int get_number_nights (RESERVATION *r) {
    int begin_days = days_from_epoch(r->begin_date);
    int end_days = days_from_epoch(r->end_date);
    return end_days - begin_days;
}

// Calcula o custo total da reserva.
double total_cost_reservation(RESERVATION *r) {
    double subtotal = r->price_per_night * r->nights;
    double taxAmount = (subtotal * r->city_tax) / 100.0;
    return subtotal + taxAmount;
}

// Insere uma reserva em um catálogo de reservas e atualiza as informações do usuário e do hotel associadas à reserva.
void insert_Reservation(CATALOG_RESERVATIONS *cr, RESERVATION *r, CATALOG_USERS *cu) {
    g_hash_table_insert(cr->reservations_hash, r->id, r);
    HOTEL_RESERVATIONS *hotel = g_hash_table_lookup(cr->hotel_reservations, r->hotel_id);
    if (hotel == NULL)
    {
        hotel = malloc(sizeof(HOTEL_RESERVATIONS));
        hotel->rates_sum = 0;
        hotel->rates_num = 0;
        hotel->reservations_total = 0;
        hotel->size = 1000;
        hotel->reservations = malloc(hotel->size * sizeof(RESERVATION *));
        hotel->hotels_sorted = NULL;
        g_hash_table_insert(cr->hotel_reservations, r->hotel_id, hotel);
    }
    if (hotel->reservations_total >= hotel->size)
    {
        hotel->size *= 2;
        hotel->reservations = realloc(hotel->reservations, hotel->size * sizeof(RESERVATION *));
    }
    hotel->reservations[hotel->reservations_total++] = r;
    hotel->rates_num++;
    hotel->rates_sum += r->rating;
    r->nights = get_number_nights(r);
    r->total_cost = total_cost_reservation(r);
    update_User_reservas(cu, r->user_id, r->total_cost);
}

// Compara duas reservas.
int compareReservations(const void *a, const void *b) {
    RESERVATION *resA = *(RESERVATION **)a;
    RESERVATION *resB = *(RESERVATION **)b;
    if (resA->begin_date.year != resB->begin_date.year)
        return resB->begin_date.year - resA->begin_date.year;
    if (resA->begin_date.month != resB->begin_date.month)
        return resB->begin_date.month - resA->begin_date.month;
    if (resA->begin_date.day != resB->begin_date.day)
        return resB->begin_date.day - resA->begin_date.day;
    return strcmp(resA->id, resB->id);
}

GHashTable* get_reservations_hash(CATALOG_RESERVATIONS *cr) {
    return cr->reservations_hash;
}

char* get_reservation_id(RESERVATION *r) {
    return r->id;
}

char *get_hotel_id(RESERVATION *r) {
    return r->hotel_id;
}

char *get_hotel_name(RESERVATION *r) {
    return r->hotel_name;
}

int get_hotel_stars(RESERVATION *r) {
    return r->hotel_stars;
}

DATE get_begin_date(RESERVATION *r) {
    return r->begin_date;
}

DATE get_end_date(RESERVATION *r) {
    return r->end_date;
}

char *get_includes_breakfast(RESERVATION *r) {
    return r->includes_breakfast;
}

int get_nights(RESERVATION *r) {
    return r->nights;
}

double get_total_cost(RESERVATION *r) {
    return r->total_cost;
}

char* get_user_id_reservation(RESERVATION* res) {
    return res->user_id;
}

GHashTable* get_hotel_reservations(CATALOG_RESERVATIONS* cr) {
    return cr->hotel_reservations;
}

int get_rates_num(HOTEL_RESERVATIONS *hr) {
    return hr->rates_num;
}

int get_rates_sum(HOTEL_RESERVATIONS* hr) {
    return hr->rates_sum;
}

int get_reservation_rating(RESERVATION *r) {
    return r->rating;
}

int get_reservations_total(HOTEL_RESERVATIONS *hr) {
    return hr->reservations_total;
}

RESERVATION** get_reservations(HOTEL_RESERVATIONS *hr) {
    return hr->reservations;
}

int get_price_per_night(RESERVATION *reservation) {
    return reservation->price_per_night;
}
