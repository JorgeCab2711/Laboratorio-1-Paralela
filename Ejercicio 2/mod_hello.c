#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void openmp_hello(int age);

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Uso: ./ejecutable <SU EDAD>\\n");
        exit(1);
    }

    int age = strtol(argv[1], NULL, 10);
    int thread_count = omp_get_max_threads();  // Getting the maximum number of threads available

#pragma omp parallel num_threads(thread_count)
    openmp_hello(age);

    return 0;
}

void openmp_hello(int age) {
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    if (my_rank % 2 == 0) {
        printf("Saludos del hilo %d\\n", my_rank);
    } else {
        printf("Feliz cumpleaños número %d !\\n", age);
    }

    printf("Hilo %d de %d\\n", my_rank, thread_count);

} /* openmp_hello */