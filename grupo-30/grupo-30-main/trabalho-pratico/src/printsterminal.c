#include "../includes/queries.h"
#include "../includes/prints.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printOneLine_Terminal (char *output){
    if (output != NULL)
    {
        printf("%s\n", output);
        free(output);
    }
    else
        printf("Não existem resultados para esta procura\n");
}

void printMultipleLines_Terminal (char **output, int lines) {
    if (output == NULL) {
        printf("Não existem resultados para esta procura!\n");
        return;
    }

    const int linesPerPage = 50;
    int currentPage = 1;
    int totalPages = (lines + linesPerPage - 1) / linesPerPage;

    while (1) {
        printf("Você está na página %d. A resposta vai até a página %d.\n", currentPage, totalPages);
        printf("Escolha a página que quer ver ou 0 para Sair!\n");

        char choice[10];
        fgets(choice, sizeof(choice), stdin);
        int selectedPage = (int) strtol(choice, NULL, 10);

        if (selectedPage > 0 && selectedPage <= totalPages) {
            int start = (selectedPage - 1) * linesPerPage;
            int end = start + linesPerPage;
            for (int i = start; i < end && i < lines; i++) {
                printf("%s\n", output[i]);
            }
            currentPage = selectedPage;
        }
        else if (selectedPage == 0)
            break;
    }
    for (int i = 0; i < lines; i++) {
        free(output[i]);
    }
    free(output);
}

int printQ1F_Terminal(char *output, int output_number, int output_type) {
    if (output == NULL) {
        printf("Não existem resultados para esta procura!\n");
        return 1;
    }
    printf("--- 1 ---\n");
    char *token = strtok(output, ";");
    int Index = 0;
    if (output_type == 1) {
        while (token != NULL) {
            printf("%s: %s\n", getUserFieldName(Index), token);
            Index++;
            token = strtok(NULL, ";");
        }
    } else if (output_type == 0) {
        while (token != NULL) {
            printf("%s: %s\n", getReservationFieldName(Index), token);
            Index++;
            token = strtok(NULL, ";");
        }
    } else if (output_type == 2) {
        while (token != NULL) {
            printf("%s: %s\n", getFlightFieldName(Index), token);
            Index++;
            token = strtok(NULL, ";");
        }
    } else {
        fprintf(stderr, "Erro: Tipo de saída desconhecido\n");
    }

    free(output);
    return 0;
}

int printQ3F_Terminal(char *output, int output_number) {
    if (output != NULL) {
        printf("--- %d ---\nrating: %s\n", output_number, output);
        return 1;
    }
    return 0;
}

void printQ4F_Terminal(char **output, int N, int output_number) {
    if (output == NULL || N <= 0)
        return;

    int v_quatro = 1;
    for (int i = 0; i < N; i++) {
        if (output[i] != NULL) {
            char *token = strtok(output[i], ";");
            printf("--- %d ---\n", v_quatro);
            v_quatro++;
            int fieldIndex = 1;

            while (token != NULL) {
                printf("%s: %s\n", getQ4FieldName(fieldIndex), token);

                token = strtok(NULL, ";");
                fieldIndex++;
            }
            if (i + 1 < N)
                printf("\n");

        } else
            fprintf(stderr, "Warning: Skipping NULL output at index %d\n", i);
        free(output[i]);
    }
    free(output);
}

void printQ5F_Terminal(char **output, int N, int output_number) {
    if (output == NULL || N <= 0)
        return;

    int v_cinco = 1;
    for (int i = 0; i < N; i++) {
        if (output[i] != NULL) {
            char *token = strtok(output[i], ";");
            printf("--- %d ---\n", v_cinco);
            v_cinco++;
            int fieldIndex = 1;

            while (token != NULL) {
                printf("%s: %s\n", getQ5FieldName(fieldIndex), token);

                token = strtok(NULL, ";");
                fieldIndex++;
            }

            if (i + 1 < N) {
                printf("\n");
            }
        } else {
            fprintf(stderr, "Warning: Skipping NULL output at index %d\n", i);
        }
        free(output[i]);
    }
    free(output);
}

void printQ7F_Terminal(char **output, int N, int output_number) {
    if (output == NULL || N <= 0)
        return;

    int v_nove = 1;
    for (int i = 0; i < N; i++) {
        if (output[i] != NULL) {
            char *token = strtok(output[i], ";");
            printf("--- %d ---\n", v_nove);
            v_nove++;
            int fieldIndex = 1;

            while (token != NULL) {
                printf("%s: %s\n", getQ7FieldName(fieldIndex), token);

                token = strtok(NULL, ";");
                fieldIndex++;
            }

            if (output[i + 1] != NULL) {
                printf("\n");
            }
        }
        free(output[i]);
    }
    free(output);
}

void printQ8F_Terminal(char *output, int output_number) {
    if (output != NULL) {
        printf("--- %d ---\nrevenue: %s\n", output_number, output);
        free(output);
    }
}

void printQ9F_Terminal(char **output, int N, int output_number) {
    if (output == NULL || N <= 0)
        return;

    int v_nove = 1;
    for (int i = 0; i < N; i++) {
        if (output[i] != NULL) {
            char *token = strtok(output[i], ";");
            printf("--- %d ---\n", v_nove);
            v_nove++;
            int fieldIndex = 1;

            while (token != NULL) {
                printf("%s: %s\n", getQ9FieldName(fieldIndex), token);

                token = strtok(NULL, ";");
                fieldIndex++;
            }
            if (i + 1 < N)
                printf("\n");
        } else
            fprintf(stderr, "Warning: Skipping NULL output at index %d\n", i);
        free(output[i]);
    }
    free(output);
}