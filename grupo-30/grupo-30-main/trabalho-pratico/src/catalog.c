#include "../includes/catalog.h"
#include "../includes/prints.h"
#include "../includes/printsterminal.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>

typedef struct catalog {
    CATALOG_FLIGHTS *cf;
    CATALOG_PASSENGERS *cp;
    CATALOG_RESERVATIONS *cr;
    CATALOG_USERS *cu;
} CATALOG;

// Função que cria o catálogo inicializando os seus componentes (Cátalogo de usuários, Cátalogo de voos, Cátalogo de reservas, Cátalogo de passageiros).
CATALOG *create_Catalog(char *entry_files) {
    CATALOG *c = malloc(sizeof(struct catalog));
    char open_users[50];
    char open_flights[50];
    char open_reservations[50];
    char open_passengers[50];

    strcpy(open_users, entry_files);
    strcpy(open_flights, entry_files);
    strcpy(open_reservations, entry_files);
    strcpy(open_passengers, entry_files);
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    FILE *fpu = fopen(strcat(open_users, "/users.csv"), "r");
    if (!fpu)
    {
        perror("Erro a abrir users.csv");
        return NULL;
    }
    CATALOG_USERS *cu = init_CatalogUsers();

    char *lineu = NULL;
    size_t lenu = 0;
    while (getline(&lineu, &lenu, fpu) > 0)
    {
        lineu[strcspn(lineu, "\n")] = 0;
        USER *u = create_User(lineu);
        if (u != NULL) insert_Catalog_User(cu, u);
    }
    printf("Users: %d\n", g_hash_table_size(get_users_hash(cu)));
    free(lineu);
    fclose(fpu);
    c->cu = cu;


    FILE *fpf = fopen(strcat(open_flights, "/flights.csv"), "r");
    if (!fpf) {
        perror("Erro a abrir flights.csv");
        return NULL;
    }
    CATALOG_FLIGHTS *cf = init_CatalogFlights();

    char *linef = NULL;
    size_t lenf = 0;

    start = clock();
    while (getline(&linef, &lenf, fpf) != -1) {
        linef[strcspn(linef, "\n")] = '\0';
        FLIGHT *f = create_Flight(linef);
        if (f != NULL)
            insert_Catalog_Flight(cf, f);
    }
    printf("Flights: %d\n", g_hash_table_size(get_flights_hash(cf)));
    printf("Airports: %d\n", g_hash_table_size(get_airports_flights(cf)));
    free(linef);
    fclose(fpf);
    c->cf = cf;
    FILE *fpr = fopen(strcat(open_reservations, "/reservations.csv"), "r");
    if (!fpr)
    {
        perror("Error a abrir reservations.csv");
        return NULL;
    }
    CATALOG_RESERVATIONS *cr = init_CatalogReservations();

    char *liner = NULL;
    size_t lenr = 0;
    while (getline(&liner, &lenr, fpr) > 0)
    {
        liner[strcspn(liner, "\n")] = 0;
        RESERVATION *r = create_Reservation(liner, c->cu);
        if (r != NULL) insert_Reservation(cr, r, c->cu);
    }
    printf("Reservations: %d\n", g_hash_table_size(get_reservations_hash(cr)));
    printf("Hotels: %d\n", g_hash_table_size(get_hotel_reservations(cr)));
    free(liner);
    fclose(fpr);
    c->cr = cr;

    FILE *fpp = fopen(strcat(open_passengers, "/passengers.csv"), "r");
    if (!fpp) {
        perror("Error opening passengers.csv");
        return NULL;
    }
    CATALOG_PASSENGERS *cp = init_CatalogPassengers();

    char *linep = NULL;
    size_t lenp = 0;
    while (getline(&linep, &lenp, fpp) > 0) {
        linep[strcspn(linep, "\n")] = 0;
        PASSENGER *p = create_Passenger(linep, c->cu, c->cf);
        if (p != NULL) insert_Catalog_Passenger(cp, p, c->cu, c->cf);
    }
    printf("Passengers: %d\n", g_hash_table_size(get_passengers_hash(cp)));
    free(linep);
    fclose(fpp);
    c->cp = cp;
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Parse of data done in: %f seconds\n", cpu_time_used);
    return c;
}

// Função que apaga o catálogo libertando a memória de todos os componentes e do próprio catálogo.
void delete_Catalog(CATALOG *c) {
    printf("Deleting Catalog Passenger\n");
    delete_Catalog_Passenger(c->cp);
    printf("Deleting Catalog Flight\n");
    delete_Catalog_Flight(c->cf);
    printf("Deleting Catalog Reservation\n");
    delete_Catalog_Reservation(c->cr);
    printf("Deleting Catalog User\n");
    delete_Catalog_User(c->cu);
    printf("Freeing Catalog\n");
    free(c);
}

/* Função responsável por realizar a Query 1 
   Com base num identificador r0_u1_f2 (reservation0, user1 ou flight2)
   utiliza funções específicas para lidar com reservas, usuários ou voos dentro desse catálogo. */
char *Q1(CATALOG *c, char *id) {
    int r0_u1_f2 = auxQ1(id);
    char *result = NULL;
    if (r0_u1_f2 == 0)
    { 
        RESERVATION *reservation = g_hash_table_lookup(get_reservations_hash(c->cr), id);  
        result = Q1_reservation(reservation);
    }
    else if (r0_u1_f2 == 1)
    {
        USER *user = g_hash_table_lookup(get_users_hash(c->cu), id);
        result = Q1_user(user);
    }
    else if (r0_u1_f2 == 2)
    {
        int idf = (int) strtol(id, (char **) NULL, 10);    
        FLIGHT *flight = g_hash_table_lookup(get_flights_hash(c->cf), &idf);
        result = Q1_flight(flight);
    }
    return result;
}

// Função auxiliar para a Query 1 que determina o identificador r0_u1_f2.
int auxQ1(char *id) {
    int r0_u1_f2 = 1;
    if (g_str_has_prefix(id, "Book"))
        r0_u1_f2 = 0;
    else
    {
        gboolean isFlightId = TRUE;
        for (gchar *ch = id; *ch != '\0'; ++ch)
        {
            if (!g_ascii_isdigit(*ch))
            {
                isFlightId = FALSE;
                break;
            }
        }
        if (isFlightId)
            r0_u1_f2 = 2;
    }
    return r0_u1_f2;
}

// Função responsável por realizar a Query 2.
ID_DATE_TYPE** Q2(CATALOG *c, char *id) {
    return flights_reservations_id_date_type(c->cu,c->cf,c->cr,id);
}

// Função responsável por realizar a Query 3.
char *Q3 (char *id_hotel, CATALOG *c) {
    return average_rate(c->cr, id_hotel);
}

// Função responsável por realizar a Query 4.
char **Q4 (CATALOG *c, char *id, int *N) {
    return list_by_hotel(c->cr, id, N);
}

// Função responsável por realizar a Query 5.
char **Q5 (CATALOG *c, char* origin, char *begindate, char *enddate, int *N) {
    DATE_SECS begin_date = convertStringToDateSecs(begindate);
    DATE_SECS end_date = convertStringToDateSecs(enddate);
    return filterAndSortFlights(c->cf, origin, begin_date, end_date, N);
}

// Função responsável por realizar a Query 6.
char **Q6 (CATALOG *c, char *year, int *N) {
    return topNAirportsYear(c->cf, atoi(year), N);
}

// Função responsável por realizar a Query 7.
char **Q7 (CATALOG *c, int *N) {
    return topNAirportsMedianDelays(c->cf,N);
}

// Função responsável por realizar a Query 8.
char *Q8(CATALOG *c, char *hotel_id, char *begindate, char *enddate) {
    return hotel_recipe(c->cr, hotel_id, begindate, enddate);
}

// Função responsável por realizar a Query 9.
char **Q9 (CATALOG *c, char *prefix, int *N) {
    return listByPrefix(c->cu, prefix, N);
}

// Função responsável pelo Parsing, chamando a query respetiva e o seu print.
void parseQueries(char *line, CATALOG *c, int outputnumber) { 
    char *i = strsep (&line, " ");
    if (strcmp(i, "1") == 0)
    {
        char *id = strsep(&line, " ");
        char *output = Q1(c, id);
        printOneLine(output, outputnumber);

    }

    else if (strcmp(i, "1F") == 0)
    {
        char *id = strsep(&line, " ");
        char *output = Q1(c,id);
        int outputtype = auxQ1(id);
        printQ1F(output, outputnumber, outputtype);
    }
    
    else if (strcmp(i, "2") == 0)
    {
        char *id = strsep(&line, " ");
        char *type = strsep(&line, " ");
        ID_DATE_TYPE** output = Q2(c,id);
        printQ2(output, outputnumber, type);
    }

    else if (strcmp(i, "2F") == 0)
    {
        char *id = strsep(&line, " ");
        char *type = strsep(&line, " ");
        ID_DATE_TYPE** output = Q2(c,id);
        printQ2F(output, outputnumber, type);

    }
    else if (strcmp(i, "3") == 0)
    {
        char *id_hotel = strsep(&line, " ");
        char *output = Q3(id_hotel, c);

        printOneLine(output, outputnumber);
    }
    else if (strcmp(i, "3F") == 0)
    {
        char *id_hotel = strsep(&line, " ");
        char *output = Q3(id_hotel, c);
        printQ3F(output, outputnumber);
    }
    else if (strcmp(i, "4") == 0)
    {
        char *id_hotel = strsep(&line, " ");
        int N;
        char **output = Q4(c, id_hotel, &N);
        printMultipleLines(output, N, outputnumber);
    }
    else if (strcmp(i, "4F") == 0)
    {
        char *id_hotel = strsep(&line, " ");
        int N;
        char **output = Q4(c, id_hotel, &N);
        printQ4F(output,N, outputnumber);
    }
    
    else if (strcmp(i, "5") == 0) 
    {
        char *origin = strsep(&line, " ");
        strsep(&line, "\"");
        char *begindate = strsep(&line, "\"");
        strsep(&line, "\"");
        char *enddate = strsep(&line, "\"");
        int N;
        char **output = Q5(c, origin, begindate, enddate, &N);
        printMultipleLines(output,N, outputnumber);
    }

    else if (strcmp(i, "5F") == 0)
    {
        char *origin = strsep(&line, " ");
        strsep(&line, "\"");
        char *begindate = strsep(&line, "\"");
        strsep(&line, "\"");
        char *enddate = strsep(&line, "\"");
        int N;
        char **output = Q5(c, origin, begindate, enddate, &N);
        printQ5F(output, N, outputnumber);
    }

    else if (strcmp(i, "6") == 0) 
    {
        char *year = strsep(&line, " ");
        char *N_char = strsep(&line, " ");
        int N = atoi(N_char);
        char **output = Q6(c, year, &N);
        printMultipleLines(output,N,outputnumber);
    }

    else if (strcmp(i, "7") == 0)
    {
        int N = atoi(strsep(&line, " "));
        char **output = Q7(c, &N);
        printMultipleLines(output, N, outputnumber);
    }

    else if (strcmp(i, "7F") == 0)
    {
        int N = atoi(strsep(&line, " "));
        char **output = Q7(c, &N);
        printQ7F(output, N, outputnumber);
    }

    else if (strcmp(i, "8") == 0)
    {
        char *id = strsep(&line, " ");
        char *begindate = strsep(&line, " ");
        char *enddate = strsep(&line, " ");
        char *output = Q8(c, id, begindate, enddate);
        printOneLine(output, outputnumber);
    }
    else if (strcmp(i, "8F") == 0)
    {
        char *id = strsep(&line, " ");
        char *begindate = strsep(&line, " ");
        char *enddate = strsep(&line, " ");
        char *output = Q8(c, id, begindate, enddate);
        printQ8F(output, outputnumber);
    }
    else if (strcmp(i, "9") == 0)
    {
        if(line[0] == '\"'){
            const char delim[2] = "\"";
            strsep(&line, delim);
            char *prefix = strsep(&line, delim);
            int N =0;
            char **output = Q9(c, prefix, &N);
            printMultipleLines(output, N, outputnumber);
        }
        else{
            char *prefix = strsep(&line, " ");
            int N =0;
            char **output = Q9(c, prefix, &N);
            printMultipleLines(output, N, outputnumber);
        }
    }
    else if (strcmp(i, "9F") == 0){
        if(line[0] == '\"'){
            const char delim[2] = "\"";
            strsep(&line, delim);
            char *prefix = strsep(&line, delim);
            int N =0;
            char **output = Q9(c, prefix, &N);
            printQ9F(output, N, outputnumber);
        }
        else{
            char *prefix = strsep(&line, " ");
            int N =0;
            char **output = Q9(c, prefix, &N);
            printQ9F(output, N, outputnumber);
        }
    }
    else
        printf("%s query desconhecida\n", i);
}

// Função responsável pela execução em Batch Mode.
int run_batch(char **efile) {
    CATALOG *c = create_Catalog(efile[1]);
    if (c == NULL) {
        fprintf(stderr, "Error creating catalog\n");
        return -1;
    }
    FILE *fp;
    fp = fopen(efile[2], "r");
    if (!fp) {
        perror("Error on batch mode");
        return -1;
    }
    int outputnumber = 1;
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fp) != -1) {
        line[strcspn(line, "\n")] = 0;
        parseQueries(line, c, outputnumber);
        outputnumber++;
    }
    printf("acabou queries\n");
    free(line);
    fclose(fp);
    delete_Catalog(c);
    return 0;
}

int run_interactiveMode() {
    char path[250];

    printf("Digite o caminho do dataset: ");
    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\n")] = 0;
    CATALOG *c = create_Catalog(path);
    if (c == NULL) {
        fprintf(stderr, "Error creating catalog\n");
        return -1;
    }

    if (c->cu == NULL || c->cf == NULL || c->cr == NULL || c->cp == NULL) {
        delete_Catalog(c);
        return -1;
    }

    int loop = 1;
    while (loop)
    {
        printf("Escolha uma das queries (1, 1F, ..., 10F) ou 0 para sair\n");
        char queryType[4];
        fgets(queryType, sizeof(queryType), stdin);
        queryType[strcspn(queryType, "\n")] = 0;
        if (strcmp(queryType, "0") == 0)
            loop = 0;
        if (strcmp(queryType, "1") == 0)
        {
            printf("Escolha o id do User, Flight ou Reservation\n");
            char id[40];
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = 0;
            char *output = Q1(c, id);
            printOneLine_Terminal(output);
            break;
        }

        else if (strcmp(queryType, "1F") == 0)
        {
            printf("Escolha o id do User, Flight ou Reservation\n");
            char id[40];
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = 0;
            char *output = Q1(c,id);
            int outputtype = auxQ1(id);
            printQ1F_Terminal(output, 1, outputtype);
            break;
        }

        else if (strcmp(queryType, "2") == 0)
        {
            printf("Escolha o id do User e 'flights' ou 'reservations', caso queira ser mais específico\n");
            char id[10], type[20];
            fgets(id, sizeof(id), stdin);
            fgets(type, sizeof(type), stdin);
            id[strcspn(id, "\n")] = 0;
            type[strcspn(type, "\n")] = 0;
            int N = 0;
            ID_DATE_TYPE** output = Q2(c, id);
            while (output[N] != NULL) {
                N++;
            }
            printMultipleLines_Terminal((char **) output, N);
            break;
        }

        else if (strcmp(queryType, "2F") == 0)
        {
            printf("Escolha o id do User e 'flights' ou 'reservations', caso queira ser mais específico\n");
            char id[10], type[20];
            fgets(id, sizeof(id), stdin);
            fgets(type, sizeof(type), stdin);
            id[strcspn(id, "\n")] = 0;
            type[strcspn(type, "\n")] = 0;
            ID_DATE_TYPE** output = Q2(c, id);
            printQ2F(output, 1, type);
            break;
        }

        else if (strcmp(queryType, "3") == 0) 
        {
            printf("Escolha o id do hotel para a Query 3\n");
            char id_hotel[10];
            fgets(id_hotel, sizeof(id_hotel), stdin);
            id_hotel[strcspn(id_hotel, "\n")] = '\0'; 
            char *output = Q3(id_hotel, c);
            printOneLine_Terminal(output);
            break;
        }

        else if (strcmp(queryType, "3F") == 0) 
        {
            printf("Escolha o id do hotel para a Query 3F\n");
            char id_hotel[10];
            fgets(id_hotel, sizeof(id_hotel), stdin);
            id_hotel[strcspn(id_hotel, "\n")] = '\0';
            char *output = Q3(id_hotel, c);
            printQ3F_Terminal(output, 1);
            break;
        }

        else if (strcmp(queryType, "4") == 0) 
        {
            printf("Escolha o id do hotel para a Query 4\n");
            char id_hotel[10];
            fgets(id_hotel, sizeof(id_hotel), stdin);
            id_hotel[strcspn(id_hotel, "\n")] = '\0';
            int N;
            char **output = Q4(c, id_hotel, &N);
            printMultipleLines_Terminal(output, N);
            break;
        }

        else if (strcmp(queryType, "4F") == 0) 
        {
            printf("Escolha o id do hotel para a Query 4F\n");
            char id_hotel[10];
            fgets(id_hotel, sizeof(id_hotel), stdin);
            id_hotel[strcspn(id_hotel, "\n")] = '\0';
            int N;
            char **output = Q4(c, id_hotel, &N);
            printQ4F_Terminal(output, N, 1);
            break;
        }

        else if (strcmp(queryType, "5") == 0)
        {
            printf("Escolha a origem, data de início e data de fim (separados por espaço) para a Query 5\n");
            char input[100];
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;

            char origin[10];
            char begindate[20];
            char enddate[20];

            if (sscanf(input, "%s \"%[^\"]\" \"%[^\"]\"", origin, begindate, enddate) != 3) {
                fprintf(stderr, "Erro na leitura dos argumentos da Query 5\n");
                break;
            }

            int N;
            char **output = Q5(c, origin, begindate, enddate, &N);
            printMultipleLines_Terminal(output, N);
            break;
        }

        else if (strcmp(queryType, "5F") == 0)
        {
            printf("Escolha a origem, data de início e data de fim (separados por espaço) para a Query 5F\n");
            char input[100];
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;

            char origin[10];
            char begindate[20];
            char enddate[20];

            if (sscanf(input, "%s \"%[^\"]\" \"%[^\"]\"", origin, begindate, enddate) != 3) {
                fprintf(stderr, "Erro na leitura dos argumentos da Query 5\n");
                break;
            }

            int N;
            char **output = Q5(c, origin, begindate, enddate, &N);
            printQ5F_Terminal(output, N, 1);
            break;
        }

        else if (strcmp(queryType, "6") == 0) 
        {
            printf("Escolha o ano e N (separados por espaço) para a Query 6\n");
            char year[10];
            int N;
            fgets(year, sizeof(year), stdin);
            char N_char[10];
            fgets(N_char, sizeof(N_char), stdin);
            year[strcspn(year, "\n")] = 0;
            N_char[strcspn(N_char, "\n")] = 0;
            N = atoi(N_char);
            char **output = Q6(c, year, &N);
            printMultipleLines_Terminal(output, N);
            break;
        }

        else if (strcmp(queryType, "7") == 0)
        {
            printf("Escolha o número N para a Query 7\n");
            char input[10];
            fgets(input, sizeof(input), stdin);
            int N = atoi(input);

            char **output = Q7(c, &N);
            printMultipleLines_Terminal(output, N);
            break;
        }

        else if (strcmp(queryType, "7F") == 0)
        {
            printf("Escolha o número N para a Query 7\n");
            char input[10];
            fgets(input, sizeof(input), stdin);
            int N = atoi(input);

            char **output = Q7(c, &N);
            printQ7F_Terminal(output, N, 1);
            break;
        }

        else if (strcmp(queryType, "8") == 0)
        {
            printf("Escolha o id, data de início e data de fim (separados por espaço) para a Query 8\n");
            char input[100];
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;

            char id[10];
            char begindate[20];
            char enddate[20];

            if (sscanf(input, "%s %s %s", id, begindate, enddate) != 3) {
                fprintf(stderr, "Erro na leitura dos argumentos da Query 8\n");
                break;
            }

            char *output = Q8(c, id, begindate, enddate);
            printOneLine_Terminal(output);
            break;
        }

        else if (strcmp(queryType, "8F") == 0)
        {
            printf("Escolha o id, data de início e data de fim (separados por espaço) para a Query 8F\n");
            char input[100];
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;

            char id[10];
            char begindate[20];
            char enddate[20];

            if (sscanf(input, "%s %s %s", id, begindate, enddate) != 3) {
                fprintf(stderr, "Erro na leitura dos argumentos da Query 8\n");
                break;
            }

            char *output = Q8(c, id, begindate, enddate);
            printQ8F_Terminal(output, 1);
            break;
        }
        else if (strcmp(queryType, "9") == 0) 
        {
            printf("Escolha o prefixo para a Query 9\n");
            char prefix[20];
            fgets(prefix, sizeof(prefix), stdin);
            prefix[strcspn(prefix, "\n")] = 0;
            int N;
            char **output = Q9(c, prefix, &N);
            printMultipleLines_Terminal(output, N);
            break;
        }
        else if (strcmp(queryType, "9F") == 0) 
        {
            printf("Escolha o prefixo para a Query 9F\n");
            char prefix[20];
            fgets(prefix, sizeof(prefix), stdin);
            prefix[strcspn(prefix, "\n")] = 0;
            int N;
            char **output = Q9(c, prefix, &N);
            printQ9F_Terminal(output, N, 1);
            break;
        }
        else
            printf("Query inválida. Por favor, escolha uma opção válida.\n");

    } 
    delete_Catalog(c);
    return 0;
}