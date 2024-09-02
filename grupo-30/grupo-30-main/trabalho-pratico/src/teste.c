#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>

#include "../includes/catalog.h"

struct timespec start, end;
struct rusage r_usage;

// Função que mede o tempo de execução de cada Query.
double measure_time(void (*query_function)(char*, CATALOG*, int), char* query, CATALOG* c, int output_number) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    query_function(query, c, output_number); // Chama a função de query para medir o tempo de execução

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9; // Calcula o tempo decorrido em segundos
    return elapsed_time;
}

// Função que mede a memória máxima usada pelo programa.
long measure_memory_usage() {
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    return r_usage.ru_maxrss;
}

// Função que compara os resultados com os resultados esperados
void compare_results(char *produced_file, char *expected_file, int output_number) {
    FILE *fp_produced = fopen(produced_file, "r"); // Abre o arquivo produzido para leitura
    if (!fp_produced) {
        printf("Resultados/command%d_output.txt não encontrado!\n", output_number);
        return;
    }

    FILE *fp_expected = fopen(expected_file, "r"); // Abre o arquivo esperado para leitura
    if (!fp_expected) { 
        perror("Error on expected");
        return;
    }

    // Variáveis para armazenar linhas de cada arquivo
    char *line_produced = NULL;
    size_t len_produced;
    char *line_expected = NULL;
    size_t len_expected;
    int verify = 1;

    // Compara cada linha nos arquivos
    while (getline(&line_produced, &len_produced, fp_produced) != -1) {
        getline(&line_expected, &len_expected, fp_expected);
        if (strcmp(line_produced, line_expected) != 0) {
            verify = 0;
            break;
        }
    }

    fclose(fp_produced);
    fclose(fp_expected);

    // Verifica se os resultados são iguais ou diferentes
    if (verify == 0)
        printf("O output do comando %d não é o output esperado!\n", output_number);
    else
        printf("O output do comando %d é igual ao output esperado!\n", output_number);
}

// Função que executa os testes
void run_tests(char *dataset_path, char *commands_file, char *expected_output_path) {
    CATALOG *c = create_Catalog(dataset_path);
    FILE *fp = fopen(commands_file, "r");

    if (!fp) {
        perror("Error");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    int output_number = 1;
    double total_execution_time = 0.0;

    // Limpar diretório de resultados antes dos testes
    system("rm -rf ./Resultados/*");

    // Loop para processar cada linha no arquivo de comandos
    while (getline(&line, &len, fp) != -1) {
        line[strcspn(line, "\n")] = 0;

        if (line == NULL || c == NULL) {
            fprintf(stderr, "Erro: Ponteiro nulo detectado.\n");
            return;
        }

        // Mede o tempo de execução da query e imprime o resultado
        double execution_time = measure_time(parseQueries, line, c, output_number);
        printf("Tempo de execução da Query %d: %.6f segundos\n", output_number, execution_time);
        total_execution_time += execution_time;

        char produced_file[100];
        snprintf(produced_file, 100, "Resultados/command%d_output.txt", output_number);
        char expected_file[100];
        snprintf(expected_file, 100, "%s/command%d_output.txt", expected_output_path, output_number);

        // Compara os resultados com os resultados esperados
        compare_results(produced_file, expected_file, output_number);

        output_number++;
    }

    printf("Tempo total de execução das queries: %.6f segundos\n", total_execution_time);

    // Medir e imprimir a memória máxima usada
    long max_memory = measure_memory_usage();
    printf("Memória máxima usada: %ld KB\n", max_memory);

    fclose(fp);
    free(line);
    delete_Catalog(c);
}

int main(int argc, char **argv) {
    if (argc == 4) {
        // Executa os testes com os caminhos do dataset, comandos e saída esperada
        run_tests(argv[1], argv[2], argv[3]);
        return 0;
    } else {
        fprintf(stderr, "Uso: %s <caminho_dataset> <caminho_comandos> <caminho_saida_esperada>\n", argv[0]);
        return 1;
    }
}