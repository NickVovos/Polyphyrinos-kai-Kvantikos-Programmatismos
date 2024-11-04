#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char* argv[]) {
    long N = 0;

    srand(time(0));

    // 1.000.000.000 is a good number
    printf("Enter the value of N: ");
    scanf("%ld", &N);

    int *A = malloc(N * sizeof(int));
    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (long i = 0; i < N; i++) {
        A[i] = rand() % 1000000;
    }

    double start_time_parallel = omp_get_wtime();
    long sum_parallel = 0;
    #pragma omp parallel for reduction(+:sum_parallel)
    for (long i = 0; i < N; i++) {
        sum_parallel += A[i];
    }
    double end_time_parallel = omp_get_wtime();
    double elapsed_time_parallel = end_time_parallel - start_time_parallel;
    printf("Parallel sum is: %ld\n", sum_parallel);
    printf("Parallel execution time: %.6f seconds\n", elapsed_time_parallel);

    double start_time = omp_get_wtime();
    long sum_sequential = 0;
    
    for (long i = 0; i < N; i++) {
        sum_sequential += A[i];
    }

    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;
    printf("Sequential execution time: %.6f seconds\n", elapsed_time);
    printf("Sequential sum is: %ld\n", sum_sequential);


    if (sum_parallel != sum_sequential) {
        printf("Error: Parallel and sequential sums do not match!\n");
    }

    double speedup = elapsed_time / elapsed_time_parallel;
    int num_threads = omp_get_max_threads();
    double efficiency = speedup / num_threads;

    printf("Speedup: %.2f\n", speedup);
    printf("Efficiency: %.2f\n", efficiency);

    free(A);

    return 0;
}
