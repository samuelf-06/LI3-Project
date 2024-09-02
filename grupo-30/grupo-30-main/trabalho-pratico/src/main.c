#include <time.h>
#include <stdio.h>
#include <sys/resource.h>

#include "../includes/catalog.h"

struct timespec start, end;
struct rusage r_usage;

//Run Batch Mode
int main(int argc, char *argv[]){
    system("rm -rf ./Resultados/*");
    if (argc == 3)
    {
        double elapsed;
        clock_gettime(CLOCK_REALTIME, &start);
        run_batch(argv);
        clock_gettime(CLOCK_REALTIME, &end);
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9; 
        printf("Elapsed time: %.6f seconds\n", elapsed);
        getrusage(RUSAGE_SELF, &r_usage);
        printf("Memory usage: %ld KB\n", r_usage.ru_maxrss);
        return 0;
    }
    else if (argc == 1)
        run_interactiveMode();
    return 0;
}


