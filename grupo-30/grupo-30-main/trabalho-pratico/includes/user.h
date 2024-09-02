#ifndef USER_H
#define USER_H

#include <glib.h>

enum account_status{
    Active,
    Inactive,
    NoStatus
};

typedef struct user USER;

typedef struct catalog_users CATALOG_USERS;

void delete_User (void *data);

USER *create_User (char *line);

CATALOG_USERS *init_CatalogUsers();

void insert_Catalog_User (CATALOG_USERS *cu, USER *u);

void delete_Catalog_User(CATALOG_USERS *cu);

int exist_user (CATALOG_USERS *cu, char *id);

int is_user_inactive(CATALOG_USERS* cu, char* user_id);

void update_User_reservas(CATALOG_USERS *cu, char *id, double cost);

void update_User_flights(CATALOG_USERS *cu, char *id);

int compareUsers(gconstpointer a,gconstpointer b);

GHashTable* get_users_hash(CATALOG_USERS *cu);

char *get_name(USER *u);

char *get_sex(USER *u);

int get_age(USER *u);

char *get_country_code(USER *u);

char *get_passport(USER *u);

int get_number_of_flights(USER *u);

int get_number_of_reservations(USER *u);

double get_total_spent(USER *u);

enum account_status get_account_status(USER *u);

char* get_user_id(USER *u);

#endif //USER_H

