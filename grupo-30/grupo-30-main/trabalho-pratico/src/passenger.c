#include "../includes/passenger.h"
#include "../includes/prints.h"

#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

typedef struct passenger {
    int flight_id;
    char* user_id;
}PASSENGER;

typedef struct catalog_passenger {
    GHashTable *passengers_hash;
} CATALOG_PASSENGERS;


// Apaga um passageiro (PASSENGER) libertando a memória alocada.
void delete_Passenger(void *data) {
    PASSENGER *p = (PASSENGER *)data;
    free(p->user_id);
    free(p);
}


// Cria um passageiro e garante a validade dos dados.
PASSENGER *create_Passenger(char *line, CATALOG_USERS *cu, CATALOG_FLIGHTS *cf) {
    PASSENGER *p = malloc(sizeof(struct passenger));
    char *token;
    int i = 0, verify = 1;
    char* copy = strdup(line);
    while ((token = strsep(&line, ";")) != NULL) {
        switch (i++) {
            case 0:
                if (strlen(token) == 0) 
                    verify = 0;
                p->flight_id = (int) strtol(token, (char **) NULL, 10);
                if (!exist_fligth(cf, p->flight_id)) {
                    verify = 0;
                }
                break;
            case 1:
                if (strlen(token) == 0)
                    verify = 0;
                p->user_id = strdup(token);
                if (p->user_id == NULL) {
                    perror("Error duplicating user_id");
                    verify = 0;
                } else if (!exist_user(cu, p->user_id)) {
                    verify = 0;
                }
                break;
        }
    }
    if (verify == 0) {
        print_errors("Resultados/passengers_errors.csv", copy);
        delete_Passenger(p);
        free(copy);
        return NULL;
    }
    free(copy);
    return p;
}

CATALOG_PASSENGERS *init_CatalogPassengers() {
    CATALOG_PASSENGERS *cp = malloc(sizeof (CATALOG_PASSENGERS));
    cp->passengers_hash = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, delete_Passenger);
    return cp;
}

// Insere um passageiro em um catálogo de passageiros e associa o passageiro a um voo em um catálogo de usuários. 
// Atualiza também os voos do usuário no catálogo de usuários.
void insert_Catalog_Passenger(CATALOG_PASSENGERS *cp, PASSENGER *p, CATALOG_USERS *cu, CATALOG_FLIGHTS *cf) {
    if(!exist_Passenger(cp,p->user_id)) g_hash_table_insert(cp->passengers_hash, p->user_id, p);
    FLIGHT *f = get_flight_from_catalog(cf, &(p->flight_id));
    if (exist_user(cu, p->user_id)) {
        add_passenger_to_flight(f, p->user_id, cf);
    }
    update_User_flights(cu, p->user_id);
}


// Apaga um catálogo de passageiros (CATALOG_PASSENGERS) libertando a memória alocada dos seus componentes.
void delete_Catalog_Passenger(CATALOG_PASSENGERS *cp) {
    g_hash_table_destroy(cp->passengers_hash);
    free(cp);
}

int exist_Passenger(CATALOG_PASSENGERS *cp, char* user_id) {
    PASSENGER *p = g_hash_table_lookup(cp->passengers_hash, user_id);
    if (p != NULL) {
        return 1;
    }
    
    return 0;
}

GHashTable* get_passengers_hash(CATALOG_PASSENGERS *cp) {
    return cp->passengers_hash;
}
