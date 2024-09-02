#include "../includes/queries.h"
#include "../includes/date.h"

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>

//********************************************************
//              QUERY 1
//********************************************************

// Função auxiliar da Query 1 para voos.
char *Q1_flight(FLIGHT *f) {
    if (f == NULL)
        return NULL;
    char *result = malloc (snprintf(NULL, 0, "%s;%s;%s;%s;%d/%02d/%02d %02d:%02d:%02d;%d/%02d/%02d %02d:%02d:%02d;%d;%d",
                                    get_airline(f), get_plane_model(f), get_origin(f), get_destination(f), get_schedule_departure_date(f).year,
                                    get_schedule_departure_date(f).month, get_schedule_departure_date(f).day, get_schedule_departure_date(f).hour,
                                    get_schedule_departure_date(f).minutes, get_schedule_departure_date(f).seconds, get_schedule_arrival_date(f).year,
                                    get_schedule_arrival_date(f).month, get_schedule_arrival_date(f).day, get_schedule_arrival_date(f).hour,
                                    get_schedule_arrival_date(f).minutes, get_schedule_arrival_date(f).seconds, get_numPassengers(f), get_delay(f)) +1);
    if (result == NULL)
        return NULL;
    if (result != NULL) {
        snprintf(result, 256, "%s;%s;%s;%s;%d/%02d/%02d %02d:%02d:%02d;%d/%02d/%02d %02d:%02d:%02d;%d;%d",
                 get_airline(f), get_plane_model(f), get_origin(f), get_destination(f), get_schedule_departure_date(f).year,
                 get_schedule_departure_date(f).month, get_schedule_departure_date(f).day, get_schedule_departure_date(f).hour,
                 get_schedule_departure_date(f).minutes, get_schedule_departure_date(f).seconds, get_schedule_arrival_date(f).year,
                 get_schedule_arrival_date(f).month, get_schedule_arrival_date(f).day, get_schedule_arrival_date(f).hour,
                 get_schedule_arrival_date(f).minutes, get_schedule_arrival_date(f).seconds, get_numPassengers(f), get_delay(f));
    }
    return result;
}


// Função auxiliar da Query 1 para reservas.
char *Q1_reservation(RESERVATION *r) {
    if (r == NULL)
        return NULL;
    int size = snprintf(NULL, 0, "%s;%s;%d;%d/%02d/%02d;%d/%02d/%02d;%s;%d;%.3lf",
                        get_hotel_id(r), get_hotel_name(r), get_hotel_stars(r), get_begin_date(r).year,
                        get_begin_date(r).month, get_begin_date(r).day, get_end_date(r).year,
                        get_end_date(r).month, get_end_date(r).day, get_includes_breakfast(r),
                        get_number_nights(r), get_total_cost(r)) +1;
    char *result = malloc(size);
    if (result == NULL)
        return NULL;
    if (result != NULL) {
        snprintf(result, size, "%s;%s;%d;%d/%02d/%02d;%d/%02d/%02d;%s;%d;%.3lf",
                 get_hotel_id(r), get_hotel_name(r), get_hotel_stars(r), get_begin_date(r).year, get_begin_date(r).month,
                 get_begin_date(r).day, get_end_date(r).year, get_end_date(r).month, get_end_date(r).day,
                 get_includes_breakfast(r), get_number_nights(r), get_total_cost(r));
    }
    return result;
}


// Função auxiliar da Query 1 para utilizadores.
char *Q1_user(USER *u) {
    if (u == NULL || get_account_status(u) == Inactive){
        return NULL;
    }
    int bufferSize = snprintf(NULL, 0, "%s;%s;%d;%s;%s;%d;%d;%.3lf",
                              get_name(u), get_sex(u), get_age(u), get_country_code(u), get_passport(u),
                              get_number_of_flights(u), get_number_of_reservations(u), get_total_spent(u)) + 1;
    char *result = malloc(bufferSize);
    if (result == NULL)
        return NULL;
    if (result != NULL) {
        snprintf(result, bufferSize, "%s;%s;%d;%s;%s;%d;%d;%.3lf",
                 get_name(u), get_sex(u), get_age(u), get_country_code(u), get_passport(u), get_number_of_flights(u),
                 get_number_of_reservations(u), get_total_spent(u));
    }
    return result;
}



//********************************************************
//              QUERY 2
//********************************************************

// Funções auxiliares da Query 2.

typedef struct id_date_type {
    char* id;
    DATE date;
    char *type;
} ID_DATE_TYPE;

char *get_id(ID_DATE_TYPE *idt) {
    return idt->id;
}

DATE get_date(ID_DATE_TYPE *idt) {
    return idt->date;
}

char *get_type(ID_DATE_TYPE *idt) {
    return idt->type;
}

// Retorna uma lista de voos associadas ao ID fornecido.
GList* get_flights_with_passenger(CATALOG_FLIGHTS* cf, char* passenger_user_id) {
    GList *flights_with_passenger = NULL;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, get_flights_hash(cf));
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        FLIGHT *flight = (FLIGHT*) value;
        GList *passenger_ids = get_passenger_ids(flight);
        GList *l;
        for (l = passenger_ids; l != NULL; l = l->next) {
            if (strcmp(l->data, passenger_user_id) == 0) {
                flights_with_passenger = g_list_append(flights_with_passenger, flight);
                break;
            }
        }
    }

    return flights_with_passenger;
}


// Retorna uma lista de reservas associadas ao ID fornecido.
GList* get_reservations_with_id(CATALOG_RESERVATIONS* catalog, char* id) {
    if (catalog == NULL || get_reservations_hash(catalog) == NULL || id == NULL) {
        printf("Catalog, hash table or id is NULL.\n");
        return NULL;
    }

    if (g_hash_table_size(get_reservations_hash(catalog)) == 0) {
        printf("Hash table is empty.\n");
        return NULL;
    }

    GList* list = NULL;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, get_reservations_hash(catalog));
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        RESERVATION* res = (RESERVATION*)value;
        if (strcmp(get_user_id_reservation(res), id) == 0) { // Alterado para comparar com user_id
            list = g_list_append(list, res);
        }
    }

    if (list == NULL) {
        printf("No reservations found with the provided user id.\n");
    }

    return list;
}


/* Compara duas datas de uma estrutura ID_DATE_TYPE.
   Auxiliar para ordenar uma lista por ordem decrescente das datas. */ 
int compare_dates_ID_DATE_TYPE(const void *a, const void *b) {
    ID_DATE_TYPE *date_a = *(ID_DATE_TYPE**)a;
    ID_DATE_TYPE *date_b = *(ID_DATE_TYPE**)b;
    if (date_a->date.year != date_b->date.year)
        return date_b->date.year - date_a->date.year;
    if (date_a->date.month != date_b->date.month)
        return date_b->date.month - date_a->date.month;
    if (date_a->date.day != date_b->date.day)
        return date_b->date.day - date_a->date.day;
    if (strcmp(date_a->type, date_b->type) != 0) // Se os tipos forem diferentes.
        return strcmp(date_a->type, date_b->type); // Ordenar pelo tipo
    return strcmp(date_a->id, date_b->id); // Critério de desempate.
}



// Retorna um array de estruturas ID_DATE_TYPE que contém as informações necessárias para a Query 2 sobre voos e reservas associadas ao ID do usuário.
ID_DATE_TYPE** flights_reservations_id_date_type(CATALOG_USERS *cu, CATALOG_FLIGHTS *cf, CATALOG_RESERVATIONS *cr, char *id) {
    if(is_user_inactive(cu,id)) return NULL; 

    GList* flights = get_flights_with_passenger(cf,id);
    GList* reserv = get_reservations_with_id(cr,id);

    int total_elements = g_list_length(flights) + g_list_length(reserv);
    ID_DATE_TYPE **result = malloc((total_elements + 1) * sizeof(ID_DATE_TYPE*)); // Adicionado 1 para o elemento NULL

    int i = 0;
    for (GList *l = flights; l != NULL; l = l->next) {
        FLIGHT *flight = (FLIGHT*)l->data;
        ID_DATE_TYPE *id_date = malloc(sizeof(ID_DATE_TYPE));
        id_date->id = malloc(21 * sizeof(char)); // Alocar memória para a string, adicionado 1 para o caractere NULL
        sprintf(id_date->id, "%d", get_flight_id(flight)); // Converter o inteiro para string
        id_date->date = DATE_SECS_to_DATE(get_schedule_departure_date(flight));
        id_date->type = "flight";
        result[i++] = id_date;
    }

    g_list_free(flights);

    for (GList *l = reserv; l != NULL; l = l->next) {
        RESERVATION *reservation = (RESERVATION*)l->data;
        ID_DATE_TYPE *id_date = malloc(sizeof(ID_DATE_TYPE));
        id_date->id = get_reservation_id(reservation);
        id_date->date = get_begin_date(reservation);
        id_date->type = "reservation";
        result[i++] = id_date;
    }
    
    g_list_free(reserv);

    result[i++] = NULL; // Adicionado elemento NULL no final

    qsort(result, total_elements, sizeof(ID_DATE_TYPE*), compare_dates_ID_DATE_TYPE);

    return result;
}




//********************************************************
//              QUERY 3
//********************************************************

// Funções auxiliares da Query 3.
char *average_rate (CATALOG_RESERVATIONS *cr, char *id_hotel) {
    if (id_hotel == NULL)
        return NULL;
    HOTEL_RESERVATIONS *hr = g_hash_table_lookup(get_hotel_reservations(cr), id_hotel);
    if (hr == NULL)
        return NULL;
    double average_rating = 0.0;
    if (get_rates_sum(hr) > 0)
        average_rating = get_rates_sum(hr) /(double) get_rates_num(hr);
    int buffer = snprintf(NULL, 0, "%.3f", average_rating) +1;
    char *result = malloc(buffer);
    if (result == NULL)
        return NULL;
    snprintf(result, buffer, "%.3f", average_rating);
    return result;
}



//********************************************************
//              QUERY 4
//********************************************************

// Funções auxiliares da Query 4.
char **list_by_hotel(CATALOG_RESERVATIONS *cr, char *id, int *N) {
    HOTEL_RESERVATIONS *hotelReservations = g_hash_table_lookup(get_hotel_reservations(cr), id);
    if (hotelReservations == NULL || get_reservations_total(hotelReservations) == 0)
        return NULL;
    qsort(get_reservations(hotelReservations), get_reservations_total(hotelReservations), sizeof(RESERVATION *), compareReservations);
    char **result = malloc(get_reservations_total(hotelReservations) * sizeof(char *));
    if (result == NULL)
        return NULL;
    for (int i = 0; i < get_reservations_total(hotelReservations); i++)
    {
        RESERVATION *r = get_reservations(hotelReservations)[i];
        if (r == NULL)
            return NULL;
        int size = snprintf(NULL, 0, "%s;%d/%02d/%02d;%d/%02d/%02d;%s;%d;%.3f",
                            get_reservation_id(r), 
                            get_begin_date(r).year, get_begin_date(r).month, get_begin_date(r).day,
                            get_end_date(r).year, get_end_date(r).month, get_end_date(r).day, 
                            get_user_id_reservation(r), 
                            get_reservation_rating(r), 
                            get_total_cost(r)) + 1;
        char *formattedReservation = malloc(size);
        if (formattedReservation == NULL)
        {
            for (int j = 0; j < i; j++)
                free(result[j]);
            free(result);
            return NULL;
        }
        snprintf(formattedReservation, size, "%s;%d/%02d/%02d;%d/%02d/%02d;%s;%d;%.3f",
                 get_reservation_id(r), 
                 get_begin_date(r).year, get_begin_date(r).month, get_begin_date(r).day,
                 get_end_date(r).year, get_end_date(r).month, get_end_date(r).day, 
                 get_user_id_reservation(r), 
                 get_reservation_rating(r), 
                 get_total_cost(r));

        result[i] = formattedReservation;
    }
    *N = get_reservations_total(hotelReservations);
    return result;
}



//********************************************************
//              QUERY 5
//********************************************************

// Funções auxiliares da Query 5.
FLIGHT** get_all_flights(CATALOG_FLIGHTS* catalog) {
    int size = g_hash_table_size(get_flights_hash(catalog));
    FLIGHT** all_flights = malloc(size * sizeof(FLIGHT*));
    GHashTableIter iter;
    gpointer key, value;
    int i = 0;

    g_hash_table_iter_init(&iter, get_flights_hash(catalog));
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        all_flights[i] = (FLIGHT*)value;
        i++;
    }

    return all_flights;
}

char **filterAndSortFlights(CATALOG_FLIGHTS *cf, char *origin, DATE_SECS begin_date, DATE_SECS end_date, int *N) {
    int i = 0;
    FLIGHT** flights = get_all_flights(cf);
    GList *filtered_flights = NULL;
    int size = g_hash_table_size(get_flights_hash(cf));


    for (i = 0; i < size; i++) {
        FLIGHT* f = flights[i];
        if (f != NULL && strcmp(get_origin(f), origin) == 0) {
            DATE_SECS departure_date = get_schedule_departure_date(f);
            if (compareDates_Secs(departure_date, begin_date) >= 0 && compareDates_Secs(departure_date, end_date) <= 0) {
                filtered_flights = g_list_append(filtered_flights, f);
            }
        }
    }
    
    free(flights);

    filtered_flights = g_list_sort(filtered_flights, (GCompareFunc) compareFlightsQ5);

    *N = g_list_length(filtered_flights);
    
    char **result = malloc((*N + 1) * sizeof(char*));

    for (i = 0; i < *N; i++) {
        FLIGHT* f = g_list_nth_data(filtered_flights, i);
        if(f != NULL) {
            char buffer[256];
            DATE_SECS departure_date = get_schedule_departure_date(f);
            sprintf(buffer, "%010d;%02d/%02d/%02d %02d:%02d:%02d;%s;%s;%s",
                    get_flight_id(f),
                    departure_date.year,
                    departure_date.month,
                    departure_date.day,
                    departure_date.hour,
                    departure_date.minutes,
                    departure_date.seconds,
                    get_destination(f),
                    get_airline(f),
                    get_plane_model(f));
            result[i] = strdup(buffer);
        }
    }
    g_list_free(filtered_flights);
    result[i++] = NULL;
    return result;
}


//********************************************************
//              QUERY 6
//********************************************************
// Função responsável por realizar a Query 6.

typedef struct airportyear {
    char *name;
    int num_passengers;
} AirportYear;

int compare_func(gconstpointer a, gconstpointer b) {
    AirportYear *airport1 = (AirportYear *)a;
    AirportYear *airport2 = (AirportYear *)b;

    if (airport1->num_passengers != airport2->num_passengers)
        return airport2->num_passengers - airport1->num_passengers;
    else
        return strcmp(airport1->name, airport2->name);
}


char **topNAirportsYear(CATALOG_FLIGHTS *catalog, int year, int *N) {
    int i = 0;
    GHashTableIter iter;
    gpointer key, value;
    GArray *airports = g_array_new(FALSE, FALSE, sizeof(AirportYear));

    GHashTable *airports_flights = get_airports_flights(catalog);
    g_hash_table_iter_init(&iter, airports_flights);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        AIRPORTS_FLIGHTS *airport = (AIRPORTS_FLIGHTS *)value;
        int total_passengers = 0;

        int flights_total = get_flights_total(airport);
        FLIGHT **flights = get_flights(airport);

        for (i = 0; i < flights_total; i++) {
            FLIGHT *flight = flights[i];
            if (getYear(get_schedule_departure_date(flight)) == year)
                total_passengers += get_numPassengers(flight);
        }

        AirportYear airportYear = {get_airport_name(airport), total_passengers};
        g_array_append_val(airports, airportYear);
    }

    g_array_sort(airports, compare_func);
    *N = airports->len;

    char **result = malloc(sizeof(char *) * (*N + 1));

    for (i = 0; i < *N && i < airports->len; i++) {
        AirportYear *airport = &g_array_index(airports, AirportYear, i);
        int len = snprintf(NULL, 0, "%s;%d", airport->name, airport->num_passengers);
        result[i] = malloc(len + 1);
        snprintf(result[i], len + 1, "%s;%d", airport->name, airport->num_passengers);
    }

    g_array_free(airports, TRUE);
    result[i++] = NULL;
    return result;

}

//********************************************************
//              QUERY 7
//********************************************************

typedef struct airportdelays {
    char *name;
    int delays;
} AirportDelays;

// Funções auxiliares da Query 7.
int compare_airports(gconstpointer a, gconstpointer b) {
    AirportDelays *airport1 = (AirportDelays *)a;
    AirportDelays *airport2 = (AirportDelays *)b;

    if (airport1->delays == airport2->delays) {
        return strcmp(airport1->name, airport2->name);
    }

    return airport2->delays - airport1->delays;
}

int compare_int(gconstpointer a, gconstpointer b) {
    int int_a = *((int*) a);
    int int_b = *((int*) b);

    if (int_a < int_b) return -1;
    else if (int_a == int_b) return 0;
    else return 1;
}

char **topNAirportsMedianDelays(CATALOG_FLIGHTS *cf, int *N) {
    int i = 0;
    GHashTableIter iter;
    gpointer key, value;
    GArray *airports = g_array_new(FALSE, FALSE, sizeof(AirportDelays));

    GHashTable *airports_flights = get_airports_flights(cf);
    g_hash_table_iter_init(&iter, airports_flights);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        AIRPORTS_FLIGHTS *airport = (AIRPORTS_FLIGHTS *)value;

        int flights_total = get_flights_total(airport);
        FLIGHT **flights = get_flights(airport);
        
        GArray *delays = g_array_new(FALSE, FALSE, sizeof(int));
        for (i = 0; i < flights_total; i++) {
            FLIGHT *flight = flights[i];
            int delay = get_delay(flight);
            g_array_append_val(delays, delay);
        }
        
        g_array_sort(delays, compare_int);
        
        int MedianDelays;
        if (delays->len % 2 == 0) {
            MedianDelays = (g_array_index(delays, int, delays->len/2 - 1) + g_array_index(delays, int, delays->len/2)) / 2;
        } else {
            MedianDelays = g_array_index(delays, int, delays->len/2);
        }
        
        g_array_free(delays, TRUE);

        AirportDelays airportDelays = {get_airport_name(airport), MedianDelays};
        g_array_append_val(airports, airportDelays);
    }

    g_array_sort(airports, compare_airports);
    if(*N > airports->len) *N = airports->len;

    char **result = malloc((*N + 1) * sizeof(char *));

    for (i = 0; i < *N && i < airports->len; i++) {
        AirportDelays *airport = &g_array_index(airports, AirportDelays, i);
        int len = snprintf(NULL, 0, "%s;%d", airport->name, airport->delays);
        result[i] = malloc(len + 1);
        snprintf(result[i], len + 1, "%s;%d", airport->name, airport->delays);
    }

    g_array_free(airports, TRUE);
    result[i++] = NULL;
    return result;
}

//********************************************************
//              QUERY 8
//********************************************************

// Funções auxiliares da Query 8.

// Calcula a receita total de um hotel para reservas dentro de um intervalo de datas especificado.
char *hotel_recipe(CATALOG_RESERVATIONS *cr, char *hotel_id, char *begindate, char *enddate) {
    DATE bdate = convertStringToDate(begindate);
    DATE edate = convertStringToDate(enddate);
    int totalRevenue = 0;
    HOTEL_RESERVATIONS *hotelReservations = g_hash_table_lookup(get_hotel_reservations(cr), hotel_id);
    if (hotelReservations != NULL)
    {
        for (int i = 0; i < get_reservations_total(hotelReservations); i++)
        {
            RESERVATION *reservation = get_reservations(hotelReservations)[i];
            if (reservation == NULL)
                return NULL;
            int nightsWithinRange = calculateNightsWithinRange(get_begin_date(reservation), get_end_date(reservation), bdate, edate);
            if (nightsWithinRange > 0)
                totalRevenue += nightsWithinRange * get_price_per_night(reservation);
        }
    }
    char *result = (char *)malloc(20);
    if (result == NULL)
    {
        perror("Erro de alocação de memória");
    }
    sprintf(result, "%d", totalRevenue);
    return result;
}


//********************************************************
//              QUERY 9
//********************************************************

// Funções auxiliares da Query 9.

// Verifica se o prefixo contém numeros.
int existNum(const char *str) {
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            return 1; // Se encontrar um número, retorna 1
        }
        str++;
    }
    return 0; // Se nenhum número for encontrado, retorna 0
}

// Retorna uma lista de utilizadores cujos nomes começam com um prefixo específico.
char **listByPrefix(CATALOG_USERS *cu, char *prefix, int *N) {
    if (prefix == NULL || existNum(prefix)) 
    {
        *N = 0;
        return NULL;
    }

    GList *usersList = NULL;
    GList *l;
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, get_users_hash(cu));
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        USER *user = (USER *)value;
        if (user == NULL)
        {
            return NULL;
        }
        if (g_str_has_prefix(get_name(user), prefix) && get_account_status(user) == Active)
            usersList = g_list_append(usersList, user);
    }
    usersList = g_list_sort(usersList, compareUsers);
    char **result = malloc(g_list_length(usersList) * sizeof(char *));
    if (result == NULL)
    {
        *N = 0;
        return NULL;
    }
    l = usersList;
    int i = 0;
    while (l != NULL)
    {
        USER *user = (USER *)l->data;
        int size = snprintf(NULL, 0, "%s;%s", get_user_id(user), get_name(user)) + 1;
        result[i] = malloc(size);
        if (result[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(result[j]);
            free(result);
            *N = 0;
            g_list_free(usersList);
            return NULL;
        }
        snprintf(result[i], size, "%s;%s", get_user_id(user), get_name(user));
        i++;
        l = l->next;
    }
    g_list_free(usersList);
    *N = i;
    return result;
}

//********************************************************
//              QUERY 10
//********************************************************

