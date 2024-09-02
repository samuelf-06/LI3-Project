#include "../includes/user.h"
#include "../includes/date.h"
#include "../includes/validator.h"
#include "../includes/prints.h"

#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>

typedef struct user {
    char *id;
    char *name;
    char *email;
    char *phone_number;
    DATE birth_date;
    char *sex;
    char *passport;
    char *country_code;
    char *address;
    DATE_SECS account_creation;
    char *pay_method;
    enum account_status account_status;
    int age;
    int number_of_flights;
    int number_of_reservations;
    double total_spent;
} USER;

typedef struct catalog_users {
    GHashTable *users_hash;
} CATALOG_USERS;


// Apaga um utilizador libertando a memória alocada dos seus componentes e do próprio.
void delete_User (void *data) {
    USER *u = (USER *) data;
    free(u->id);
    free(u->name);
    free(u->email);
    free(u->phone_number);
    free(u->sex);
    free(u->passport);
    free(u->country_code);
    free(u->address);
    free(u->pay_method);
    free(u);
}

// Cria uma estrutura utilizador (USER), alocando memória, e garante a validade dos dados.
USER *create_User (char *line){
    USER *u = calloc(1,sizeof (struct user));
    char *token;
    int i = 0, verify = 1;
    char *copy = strdup(line);
    while ((token = strsep(&line, ";")) != NULL)
    {
        switch(i++)
        {
            case 0:
                if (strlen(token) == 0)
                    verify = 0;
                u->id = strdup(token);
                break;
            case 1:
                if (strlen(token) == 0)
                    verify = 0;
                u->name = strdup(token);
                break;
            case 2:
                if (verify_email(token) == 0)
                    verify = 0;
                u->email = strdup(token);
                break;
            case 3:
                if (strlen(token) == 0)
                    verify = 0;
                u->phone_number = strdup(token);
                break;
            case 4:
                u->birth_date = convertStringToDate(token);
                if (verify_date(token) == 0)
                    verify = 0;
                break;
            case 5:
                if (strlen(token) == 0)
                    verify = 0;
                u->sex = strdup(token);
                break;
            case 6:
                if (strlen(token) == 0)
                    verify = 0;
                u->passport = strdup(token);
                break;
            case 7:
                if (verify_countryCode(token) == 0)
                    verify = 0;
                u->country_code = verify_countryCode(token);
                break;
            case 8:
                if (strlen(token) == 0)
                    verify = 0;
                u->address = strdup(token);
                break;
            case 9:
                u->account_creation = convertStringToDateSecs(token);
                if (verify_date_secs(token) == 0)
                    verify = 0;
                break;
            case 10:
                if (strlen(token) == 0)
                    verify = 0;
                u->pay_method = strdup(token);
                break;
            case 11:
                u->account_status = verify_AccountStatus(token);
                if (u->account_status == NoStatus)
                        verify = 0;
                break;
        }
    }
    if (verify == 0)
    {
        print_errors("Resultados/users_errors.csv", copy);
        delete_User (u);
        free(copy);
        return NULL;
    }
    u->age = calculateAge(u->birth_date);
    u->number_of_flights = 0;
    u->number_of_reservations = 0;
    free(copy);
    return u;
}

CATALOG_USERS *init_CatalogUsers() {
    CATALOG_USERS *cu = malloc(sizeof (CATALOG_USERS));
    cu->users_hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, delete_User);
    return cu;
}

// Insere um utilizador (USER) no catálogo dos utilizadores (CATALOG_USERS).
void insert_Catalog_User (CATALOG_USERS *cu, USER *u) {
    g_hash_table_insert(cu->users_hash, u->id, u);
}

// Apaga um catálogo de utilizadores libertando a memória alocada dos seus componentes e do próprio.
void delete_Catalog_User(CATALOG_USERS *cu) {
    g_hash_table_destroy(cu->users_hash);
    free(cu);
}

// Verifica a existência de um utilizador num catalógo com um certo ID.
int exist_user (CATALOG_USERS *cu, char *id){
    USER *u = g_hash_table_lookup(cu->users_hash, id);
    if (u == NULL)
        return 0;
    else
        return 1;
}

// Verifica se um usuário está inativo
int is_user_inactive(CATALOG_USERS* cu, char* user_id) {
    if (cu == NULL || cu->users_hash == NULL || user_id == NULL) {
        return -1;
    }

    USER* user = (USER*)g_hash_table_lookup(cu->users_hash, user_id);
    if (user == NULL) {
        return -1; 
    }

    return user->account_status == Inactive ? 1 : 0; // Retornar 1 se inativo, 0 caso contrário
}

// Atualiza as informações de um usuário em um catálogo de usuários com base em uma reserva feita.
void update_User_reservas(CATALOG_USERS *cu, char *id, double cost) {
    USER *u = g_hash_table_lookup(cu->users_hash, id);
    if (u == NULL)
        return;
    u->number_of_reservations ++;
    u->total_spent += cost;
}

// Atualiza as informações de um usuário em um catálogo de usuários com base em um voo feito
void update_User_flights(CATALOG_USERS *cu, char *id) {
    USER *u = g_hash_table_lookup(cu->users_hash, id);
    if (u == NULL)
        return;
    u->number_of_flights ++;
}

// Compara dois utilizadores pelo nome.
int compareUsers(gconstpointer a,gconstpointer b) {
    USER *userA = (USER *)a;
    USER *userB = (USER *)b;
    setlocale(LC_COLLATE, "en_US.UTF-8");
    int nameComparison = strcoll(userA->name, userB->name);
    if (nameComparison != 0)
        return nameComparison;
    return strcoll(userA->id, userB->id);
}



GHashTable* get_users_hash(CATALOG_USERS *cu) {
    return cu->users_hash;
}

char *get_name(USER *u) {
    return u->name;
}

char *get_sex(USER *u) {
    return u->sex;
}

int get_age(USER *u) {
    return u->age;
}

char *get_country_code(USER *u) {
    return u->country_code;
}

char *get_passport(USER *u) {
    return u->passport;
}

int get_number_of_flights(USER *u) {
    return u->number_of_flights;
}

int get_number_of_reservations(USER *u) {
    return u->number_of_reservations;
}

double get_total_spent(USER *u) {
    return u->total_spent;
}

enum account_status get_account_status(USER *u) {
    return u->account_status;
}

char* get_user_id(USER *u) {
    return u->id;
}