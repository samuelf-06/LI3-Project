#include "../includes/queries.h"
#include "../includes/prints.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Auxiliar para impressão Query1F dando return ao field a ser usado
const char *getUserFieldName(int Index) {
    static const char *userFieldNames[] = {
        "name",
        "sex",
        "age",
        "country_code",
        "passport",
        "number_of_flights",
        "number_of_reservations",
        "total_spent"
    };

    return userFieldNames[Index];
}

// Auxiliar para impressão Query1F dando return ao field a ser usado
const char *getFlightFieldName(int Index) {
    static const char *flightFieldNames[] = {
        "airline",
        "plane_model",
        "origin",
        "destination",
        "schedule_departure_date",
        "schedule_arrival_date",
        "passengers",
        "delay"
    };

    return flightFieldNames[Index++];
}

// Auxiliar para impressão Query1F dando return ao field a ser usado
const char *getReservationFieldName(int Index) {
    static const char *reservationFieldNames[] = {
        "hotel_id",
        "hotel_name",
        "hotel_stars",
        "begin_date",
        "end_date",
        "includes_breakfast",
        "nights",
        "total_price"
    };

    return reservationFieldNames[Index++];
}

// Auxiliar para impressão Query4F dando return ao field a ser usado
const char *getQ4FieldName(int fieldIndex) {
    switch (fieldIndex) {
        case 1:
            return "id";
        case 2:
            return "begin_date";
        case 3:
            return "end_date";
        case 4:
            return "user_id";
        case 5:
            return "rating";
        case 6:
            return "total_price";
        default:
            fprintf(stderr, "Error: Unknown field index\n");
            return NULL;
    }
}

// Auxiliar para impressão da Query5F dando return ao field a ser usado
const char *getQ5FieldName(int fieldIndex) {
    switch (fieldIndex) {
        case 1:
            return "id";
        case 2:
            return "schedule_departure_date";
        case 3:
            return "destination";
        case 4:
            return "airline";
        case 5:
            return "plane_model";
        default:
            fprintf(stderr, "Error: Unknown field index\n");
            return NULL;
    }
}

// Auxiliar para impressão da Query7F dando return ao field a ser usado
const char *getQ7FieldName(int fieldIndex) {
    switch (fieldIndex) {
        case 1:
            return "name";
        case 2:
            return "median";
        default:
            fprintf(stderr, "Error: Unknown field index\n");
            return NULL;
    }
}

// Auxiliar para impressão Query9F dando return ao field a ser usado
const char *getQ9FieldName(int fieldIndex) {
    switch (fieldIndex) {
        case 1:
            return "id";
        case 2:
            return "name";
        default:
            fprintf(stderr, "Error: Unknown field index\n");
            return NULL;
    }
}

// Responsável pela criação e impressão dos ficheiros resultantes das Querys em que o output é uma string, imprimindo apenas uma linha.
int printOneLine(char *output, int output_number) {
    char filename[100];
    if (output == NULL){
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
        FILE *file = fopen(filename, "w");
        fclose(file);
        return 0;
    }
    else {
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
        FILE *file = fopen(filename, "a");
        if (file == NULL) {
            fprintf(stderr, "Error opening file '%s' for writing\n", filename);
            return 0;
        }
        fprintf(file, "%s\n", output);
        free(output);
        fclose(file);
        return 1;
    }
}

// Responsável pela criação e impressão dos ficheiros resultantes das Querys em que o output é um array com as informações, imprimindo várias linhas.
int printMultipleLines(char **output, int N, int output_number) {
    char filename[100];
    if (output == NULL || N <= 0){
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
        FILE *file = fopen(filename, "w");
        fclose(file);
        return 1;
    }
    snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s' for writing\n", filename);
        return 0;
    }
    for (int i = 0; i < N; i++) {
        if (output[i] != NULL)
            fprintf(file, "%s\n", output[i]);
        free(output[i]);
    }
    free(output);
    fclose(file);
    return 1;
}

// Responsável pela criação e impressão dos ficheiros resultantes da Query1F
int printQ1F(char *output, int output_number, int output_type) {
    char filename[100];
    if (output == NULL){
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
        FILE *file = fopen(filename, "w");
        fclose(file);
        return 1;
    }
    else {
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt",output_number);
        FILE *file = fopen(filename, "a");
        if (file == NULL) {
            fprintf(stderr, "Error opening file '%s' for wriring\n", filename);
            return 0;
        }

        fprintf(file, "--- 1 ---\n");

        char *token = strtok(output, ";");
        int Index = 0;
        if (output_type == 1) {
            while (token != NULL) {
                fprintf(file, "%s: %s\n", getUserFieldName(Index),token);
                Index++;
                token = strtok(NULL, ";");
            }
        } else if (output_type == 0) {
            while (token != NULL) {
                fprintf(file, "%s: %s\n", getReservationFieldName(Index), token);
                Index++;
                token = strtok(NULL, ";");
            }
        } else if (output_type == 2) {
            while (token != NULL) {
                fprintf(file, "%s: %s\n", getFlightFieldName(Index),token);
                Index++;
                token = strtok(NULL, ";");
            }
        } else {
            fprintf(stderr, "Error: Unknown output type\n");
        }
        
        free(output);
        fclose(file);
    }
    return 0;
}

// Responsável pela criação e impressão dos ficheiros resultantes da Query2
int printQ2(ID_DATE_TYPE** id_date_type_array, int output_number, char *type) {
    char filename[100];
    if (id_date_type_array == NULL){
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
        FILE *file = fopen(filename, "w");
        fclose(file);
        return 1;      
    }
    snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s' for writing\n", filename);
        return 0;
    }
        
    if(type == NULL){
        for (int i = 0; id_date_type_array[i] != NULL; i++) {
            if(get_id(id_date_type_array[i])[0] == 'B'){
                fprintf(file,"%s;%04d/%02d/%02d;%s\n", get_id(id_date_type_array[i]), 
                                                       get_date(id_date_type_array[i]).year, get_date(id_date_type_array[i]).month, get_date(id_date_type_array[i]).day, 
                                                       get_type(id_date_type_array[i]));
            }
            else fprintf(file,"%010d;%04d/%02d/%02d;%s\n", atoi(get_id(id_date_type_array[i])), 
                                                           get_date(id_date_type_array[i]).year, get_date(id_date_type_array[i]).month, get_date(id_date_type_array[i]).day, 
                                                           get_type(id_date_type_array[i]));
            free(id_date_type_array[i]);
    
            }
        }
    
    else if(strcmp(type, "reservations") == 0){
        for (int i = 0; id_date_type_array[i] != NULL; i++) {
            if(strcmp(get_type(id_date_type_array[i]), "reservation") == 0)
                fprintf(file,"%s;%04d/%02d/%02d\n", get_id(id_date_type_array[i]), 
                                                    get_date(id_date_type_array[i]).year, get_date(id_date_type_array[i]).month, get_date(id_date_type_array[i]).day);
            free(id_date_type_array[i]);
            }
        }
    else if(strcmp(type, "flights") == 0){
        for (int i = 0; id_date_type_array[i] != NULL; i++) {
            if(strcmp(get_type(id_date_type_array[i]), "flight") == 0)
                fprintf(file,"%010d;%04d/%02d/%02d\n", atoi(get_id(id_date_type_array[i])), 
                                                       get_date(id_date_type_array[i]).year, get_date(id_date_type_array[i]).month, get_date(id_date_type_array[i]).day);
            free(id_date_type_array[i]);
            }
        }
    
    free(id_date_type_array);
    fclose(file);
    return 1;
}

// Responsável pela criação e impressão dos ficheiros resultantes da Query2F
int printQ2F(ID_DATE_TYPE** id_date_type_array, int output_number, char *type) {
    char filename[100];
    if (id_date_type_array == NULL){
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
        FILE *file = fopen(filename, "w");
        fclose(file);
        return 1;      
    }
    snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s' for writing\n", filename);
        return 0;
    }
    int v_dois = 1;
    if(type == NULL){
        for (int i = 0; id_date_type_array[i] != NULL; i++) {
            if(get_id(id_date_type_array[i])[0] == 'B'){
                if(v_dois!=1)fprintf(file,"\n");
                fprintf(file, "--- %d ---\n", v_dois++);
                fprintf(file, "id: %s\n", get_id(id_date_type_array[i]));
                fprintf(file, "date: %04d/%02d/%02d\n", get_date(id_date_type_array[i]).year, get_date(id_date_type_array[i]).month, get_date(id_date_type_array[i]).day);
                fprintf(file, "type: %s\n", get_type(id_date_type_array[i]));
            }
            else{
                if(v_dois!=1)fprintf(file,"\n");
                fprintf(file, "--- %d ---\n", v_dois++);
                fprintf(file, "id: %010d\n", atoi(get_id(id_date_type_array[i])));
                fprintf(file, "date: %04d/%02d/%02d\n", get_date(id_date_type_array[i]).year, get_date(id_date_type_array[i]).month, get_date(id_date_type_array[i]).day);
                fprintf(file, "type: %s\n", get_type(id_date_type_array[i]));
            }
            free(id_date_type_array[i]);
            }
        }
    
    else if(strcmp(type, "reservations") == 0){
        for (int i = 0; id_date_type_array[i] != NULL; i++) {
            if(strcmp(get_type(id_date_type_array[i]), "reservation") == 0){
                if(v_dois!=1)fprintf(file,"\n");
                fprintf(file, "--- %d ---\n", v_dois++);
                fprintf(file, "id: %s\n", get_id(id_date_type_array[i]));
                fprintf(file, "date: %04d/%02d/%02d\n", get_date(id_date_type_array[i]).year, get_date(id_date_type_array[i]).month, get_date(id_date_type_array[i]).day);
            }
            free(id_date_type_array[i]);
            }
        }
    else if(strcmp(type, "flights") == 0){
        for (int i = 0; id_date_type_array[i] != NULL; i++) {
            if(strcmp(get_type(id_date_type_array[i]), "flight") == 0){
                if(v_dois!=1)fprintf(file,"\n");
                fprintf(file, "--- %d ---\n", v_dois++);
                fprintf(file, "id: %010d\n", atoi(get_id(id_date_type_array[i])));
                fprintf(file, "date: %04d/%02d/%02d\n", get_date(id_date_type_array[i]).year, get_date(id_date_type_array[i]).month, get_date(id_date_type_array[i]).day);
            }
            free(id_date_type_array[i]);
            }
        }
    
    free(id_date_type_array);
    fclose(file);
    return 1;
}

// Responsável pela criação e impressão dos ficheiros resultantes da Query3F
int printQ3F(char *output, int output_number) {
    if (output != NULL) {
        char filename[100];
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
        FILE *file = fopen(filename, "a");
        if (file == NULL) {
            fprintf(stderr, "Error opening file '%s' for writing\n", filename);
            return 0;
        }
        fprintf(file, "--- 1 ---\nrating: %s\n", output);
        free(output);
        fclose(file);
        return 1;
    }
    return 0;
}

// Responsável pela criação e impressão dos ficheiros resultantes da Query4F
int printQ4F(char **output, int N, int output_number) {
    if (output == NULL || N <= 0)
        return 0;
    char filename[100];
    snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s' for writing\n", filename);
        return 0;
    }
    int v_quatro = 1;
    for(int i = 0; i < N; i++) {
        if (output[i] != NULL) {
            char *token = strtok(output[i], ";");
            fprintf(file, "--- %d ---\n", v_quatro);
            v_quatro++;
            int fieldIndex = 1;

            while (token != NULL) {
                fprintf(file, "%s: %s\n", getQ4FieldName(fieldIndex), token);

                token = strtok(NULL, ";");
                fieldIndex++;
            }
            if (i+1 <N)
                fprintf(file, "\n");

        } else
            fprintf(stderr, "Warning: Skipping NULL output at index %d\n", i);
        free(output[i]);
    }
    free(output);
    fclose(file);
    return 1;
}

// Responsável pela criação e impressão dos ficheiros resultantes da Query5F
int printQ5F(char **output, int N, int output_number) {
    if (output == NULL || N <= 0)
        return 0;

    char filename[100];
    snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s' for writing\n", filename);
        return 0;
    }
    int v_cinco = 1;
    for(int i = 0; i < N; i++) {
        if (output[i] != NULL) {
            char *token = strtok(output[i], ";");
            fprintf(file, "--- %d ---\n", v_cinco);
            v_cinco++;
            int fieldIndex = 1;

            while (token != NULL) {
                fprintf(file, "%s: %s\n", getQ5FieldName(fieldIndex), token);

                token = strtok(NULL, ";");
                fieldIndex++;
            }
            if (i+1 <N) {
                fprintf(file, "\n");
            }
        } else
            fprintf(stderr, "Warning: Skipping NULL output at index %d\n", i);
        free(output[i]);
    }
    free(output);
    fclose(file);
    return 1;
}
// Responsável pela criação e impressão dos ficheiros resultantes da Query9F
int printQ7F(char **output, int N, int output_number) {
    char filename[100];
    if (output == NULL || N <= 0){
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
        FILE *file = fopen(filename, "w");
        fclose(file);
        return 1;
    }
    snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s' for writing\n", filename);
        return 0;
    }
    int v_nove = 1;
    for(int i = 0; i < N; i++) {
        if (output[i] != NULL) {
            char *token = strtok(output[i], ";");
            fprintf(file, "--- %d ---\n", v_nove);
            v_nove++;
            int fieldIndex = 1;

            while (token != NULL) {
                fprintf(file, "%s: %s\n", getQ7FieldName(fieldIndex), token);

                token = strtok(NULL, ";");
                fieldIndex++;
            }
            if (output[i+1] != NULL){
                fprintf(file, "\n");
            }  
        }
        free(output[i]);
    }
    free(output);
    fclose(file);
    return 1;
}

// Responsável pela criação e impressão dos ficheiros resultantes da Query8F
int printQ8F (char *output, int output_number){
    if (output != NULL) {
        char filename[100];
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt",output_number);
        FILE *file = fopen(filename, "a");
        if (file == NULL) {
            fprintf(stderr, "Error opening file '%s' for wriring\n", filename);
            return 0;
        }
    
        fprintf(file, "--- 1 ---\nrevenue: %s\n", output);
        free(output);
        fclose(file);
        return 1;
    }
    return 0;
}


// Responsável pela criação e impressão dos ficheiros resultantes da Query9F
int printQ9F(char **output, int N, int output_number) {
    char filename[100];
    if (output == NULL || N <= 0){
        snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
        FILE *file = fopen(filename, "w");
        fclose(file);
        return 1;
    }
    snprintf(filename, sizeof(filename), "Resultados/command%d_output.txt", output_number);
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s' for writing\n", filename);
        return 0;
    }
    int v_nove = 1;
    for(int i = 0; i < N; i++) {
        if (output[i] != NULL) {
            char *token = strtok(output[i], ";");
            fprintf(file, "--- %d ---\n", v_nove);
            v_nove++;
            int fieldIndex = 1;

            while (token != NULL) {
                fprintf(file, "%s: %s\n", getQ9FieldName(fieldIndex), token);

                token = strtok(NULL, ";");
                fieldIndex++;
            }
            if (i+1 <N)
                fprintf(file, "\n");
        } else
            fprintf(stderr, "Warning: Skipping NULL output at index %d\n", i);
        free(output[i]);
    }
    free(output);
    fclose(file);
    return 1;
}

// Imprime os erros nos ficheiros correspondentes (flights_errors.csv,passengers_errors.csv,reservations_errors.csv,users_errors.csv).
void print_errors(const char *filename, const char *line) {
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "%s\n", line);
        fclose(file);
    } else {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita\n", filename);
    }
}