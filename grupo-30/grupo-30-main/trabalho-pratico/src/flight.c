#include "../includes/flight.h"
#include "../includes/date.h"
#include "../includes/validator.h"
#include "../includes/passenger.h"
#include "../includes/user.h"
#include "../includes/prints.h"

#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

typedef struct flight {
    int id;
    char *airline;
    char *plane_model;
    int total_seats;
    char *origin;
    char *destination;
    DATE_SECS schedule_departure_date;
    DATE_SECS schedule_arrival_date;
    DATE_SECS real_departure_date;
    DATE_SECS real_arrival_date;
    char *pilot;
    char *copilot;
    char *notes;
    int delay;
    int numPassengers;
    GList *passenger_user_ids;
} FLIGHT;

typedef struct airport {
    char *name;
    int num_passengers;
    GArray* delays;
    int size;
    int flights_total;
    FLIGHT **flights;
} AIRPORTS_FLIGHTS;

typedef struct catalog_flights {
    GHashTable *flights_hash;
    GHashTable *airports_flights;
} CATALOG_FLIGHTS;


// Apaga um voo libertando a memória alocada de cada um dos seus componentes.
void delete_Flight (void *data) {
    FLIGHT *f = (FLIGHT *) data;
    free(f->airline);
    free(f->plane_model);
    free(f->origin);
    free(f->destination);
    free(f->pilot);
    free(f->copilot);
    free(f->notes);
    g_list_free_full(f->passenger_user_ids, free);
    free(f);
}


// Cria um voo e garante a validade dos dados.
FLIGHT *create_Flight (char *line){
    FLIGHT *f = calloc(1,sizeof (struct flight));
    f->passenger_user_ids = NULL;
    char* token;
    char* copy = strdup(line);
    int i = 0, verify = 1;
    while ((token = strsep(&line, ";")) != NULL)
    {
        switch (i++)
        {
            case 0:
                if (strlen(token) == 0) verify = 0;
                f->id = (int) strtol(token, (char **) NULL, 10);
                break;
            case 1:
                if (strlen(token) == 0) verify = 0;
                f->airline = strdup(token);
                break;
            case 2:
                if (strlen(token) == 0) verify = 0;
                f->plane_model = strdup(token);
                break;
            case 3:
                if (verify_int(token) == 0) verify = 0;
                f->total_seats = (int) strtol(token, (char **) NULL, 10);
                break;
            case 4:
                if (verify_airport(token) == 0)
                    verify = 0;
                char *upper_origin = strdup(token);
                for(int i = 0; i < strlen(upper_origin); i++){
                    upper_origin[i] = toupper(upper_origin[i]);
                }
                f->origin = strdup(upper_origin);
                free(upper_origin);
                break;
            case 5:
                if (verify_airport(token) == 0)
                    verify = 0;
                char *upper_destination = strdup(token);
                for(int i = 0; i < strlen(upper_destination); i++){
                    upper_destination[i] = toupper(upper_destination[i]);
                }
                f->destination = strdup(upper_destination); 
                free(upper_destination);
                break;
            case 6:
                f->schedule_departure_date = convertStringToDateSecs(token);
                if (verify_date_secs(token) == 0)
                    verify = 0;
                break;
            case 7:
                f->schedule_arrival_date = convertStringToDateSecs(token);
                if (verify_date_secs(token) == 0)
                    verify = 0;
                break;
            case 8:
                f->real_departure_date = convertStringToDateSecs(token);
                if (verify_date_secs(token) == 0)
                    verify = 0;
                break;
            case 9:
                f->real_arrival_date = convertStringToDateSecs(token);
                if (verify_date_secs(token) == 0)
                    verify = 0;
                break;
            case 10:
                if (strlen(token) == 0) verify = 0;
                f->pilot = strdup(token);
                break;
            case 11:
                if (strlen(token) == 0) verify = 0;
                f->copilot = strdup(token);
                break;
            case 12:
                f->notes = strdup(token);
                break;
        }
    }
    if(verify != 0){
        if (secs_from_epoch(f->real_arrival_date) - secs_from_epoch(f->real_departure_date) < 0 ||
                secs_from_epoch(f->schedule_arrival_date) - secs_from_epoch(f->schedule_departure_date) < 0)
            verify = 0;
    }
    if (verify == 0)
    {
        print_errors("Resultados/flights_errors.csv",copy);
        free(copy);
        delete_Flight(f);
        return NULL;
    }
    f->delay = calculateDelay(f->schedule_departure_date, f->real_departure_date);
    f->numPassengers = 0;
    free(copy);
    return f;
}

// Libera a memória alocada para uma estrutura AIRPORTS_FLIGHTS.
void delete_airportFlights(void *data) {
    AIRPORTS_FLIGHTS *af = (AIRPORTS_FLIGHTS *)data;
    free(af->name);
    g_array_free(af->delays, TRUE);
    free(af->flights);
    free(af);
}

// Inicializa, alocando memória, uma estrutura CATALOG_FLIGHTS.
CATALOG_FLIGHTS  *init_CatalogFlights() {
    CATALOG_FLIGHTS *cf = malloc(sizeof (CATALOG_FLIGHTS));
    cf->airports_flights = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, delete_airportFlights);
    cf->flights_hash = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, delete_Flight);
    return cf;
}

// Insere um voo (FLIGHT) no catálogo dos voos (CATALOG_FLIGHTS).
void insert_Catalog_Flight (CATALOG_FLIGHTS *cf, FLIGHT *f) {
    gint *key = g_new(gint, 1);
    *key = f->id;
    g_hash_table_insert(cf->flights_hash, key, f);
    AIRPORTS_FLIGHTS *airport = g_hash_table_lookup(cf->airports_flights, f->origin);
    if (airport == NULL)
    {
        airport = malloc(sizeof (AIRPORTS_FLIGHTS));
        airport->size = 20;
        airport->flights_total = 0;
        airport->name = strdup(f->origin);
        airport->num_passengers = 0;
        airport->delays = g_array_new(FALSE, FALSE, sizeof(int));
        airport->flights = malloc(airport->size * sizeof (FLIGHT *));
        g_hash_table_insert(cf->airports_flights, strdup(f->origin), airport);
    }
    if (airport->flights_total >= airport->size)
    {
        airport->size *=2;
        airport->flights = realloc(airport->flights, airport->size * sizeof (FLIGHT *));
    }
    airport->flights[airport->flights_total++] = f;
    g_array_append_val(airport->delays, f->delay);
}


// Apaga um catálogo de voos (CATALOG_FLIGHTS) libertando a memória alocada.
void delete_Catalog_Flight(CATALOG_FLIGHTS *cf) {
    g_hash_table_destroy(cf->flights_hash);
    g_hash_table_destroy(cf->airports_flights);
    free(cf);
}

// Verifica se existe o voo com um certo ID.
int exist_fligth(CATALOG_FLIGHTS *cf, int id) {
    FLIGHT *f = g_hash_table_lookup(cf->flights_hash, &(id));
    if (f != NULL) {
        return 1;
    }
    
    return 0;
}

int compareFlightsQ5(FLIGHT *f1, FLIGHT *f2) {
    int date_comparison = compareDates_Secs(get_schedule_departure_date(f1), get_schedule_departure_date(f2));
    if (date_comparison == 0) {
        return get_flight_id(f1) - get_flight_id(f2);
    } else {
        return -date_comparison; 
    }
}

void add_passenger_to_flight(FLIGHT *f, char *user_id, CATALOG_FLIGHTS *cf) {
    f->passenger_user_ids = g_list_append(f->passenger_user_ids, strdup(user_id));
    f->numPassengers++;
    // Encontre o aeroporto correspondente no catálogo de voos
    AIRPORTS_FLIGHTS *airport = g_hash_table_lookup(cf->airports_flights, f->origin);
    if (airport != NULL) {
        // Incremente o número de passageiros do aeroporto
        airport->num_passengers++;
    }
}

FLIGHT *get_flight_from_catalog(CATALOG_FLIGHTS *cf, int *flight_id) {
    return g_hash_table_lookup(cf->flights_hash, flight_id);
}

GHashTable* get_flights_hash(CATALOG_FLIGHTS *cf) {
    return cf->flights_hash;
}

char* get_airline(FLIGHT *f) {
    return f->airline;
}

char* get_plane_model(FLIGHT *f) {
    return f->plane_model;
}

char* get_origin(FLIGHT *f) {
    return f->origin;
}

char* get_destination(FLIGHT *f) {
    return f->destination;
}

DATE_SECS get_schedule_departure_date(FLIGHT *f) {
    return f->schedule_departure_date;
}

DATE_SECS get_schedule_arrival_date(FLIGHT *f) {
    return f->schedule_arrival_date;
}

int get_numPassengers(FLIGHT *f) {
    return f->numPassengers;
}

int get_delay(FLIGHT *f) {
    return f->delay;
}

GList* get_passenger_ids(FLIGHT* flight) {
    return flight->passenger_user_ids;
}

GHashTable* get_airports_flights(CATALOG_FLIGHTS *cf) {
    return cf->airports_flights;
}

int get_flight_id(FLIGHT* flight) {
    return flight->id;
}

int get_flights_total(AIRPORTS_FLIGHTS *af) {
    return af->flights_total;
}

FLIGHT** get_flights(AIRPORTS_FLIGHTS *af) {
    return af->flights;
}

GArray* get_delays(AIRPORTS_FLIGHTS *airport) {
    return airport->delays;
}

char* get_airport_name(AIRPORTS_FLIGHTS *airport) {
    return airport->name;
}
