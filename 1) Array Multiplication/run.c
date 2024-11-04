#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char* argv[]) {

    int N = 0;
    float P = 3.14159;
    int i;
    srand(time(0));

   // 1.000.000.000 it is a good number
    while (N < 1000000) {
        printf("Please give me a number that is greater than 1,000,000: ");
        scanf("%d", &N);
    }

    int *A = malloc(N * sizeof(int));
    if (A == NULL) {
        fprintf(stderr, "Memory allocation failed for A\n");
        return 1;
    }

    double *B = malloc(N * sizeof(double));
    if (B == NULL) {
        fprintf(stderr, "Memory allocation failed for B\n");
        free(A); 
        return 1;
    }

        for (i = 0; i < N; i++) {
            A[i] = rand() % 1000;
        }

    double start_time_parallel = omp_get_wtime();
    #pragma omp parallel for private(i) shared(A, B, P, N)
    for (i = 0; i < N; i++) {
        B[i] = P * A[i];
    }
    double end_time_parallel = omp_get_wtime();
    double elapsed_time_parallel = end_time_parallel - start_time_parallel;
    printf("Parallel execution time: %.6f seconds\n", elapsed_time_parallel);

    double start_time = omp_get_wtime();
    for (i = 0; i < N; i++) {
        B[i] = P * A[i];
    }
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;
    printf("Sequential execution time: %.6f seconds\n", elapsed_time);

    double speedup = elapsed_time / elapsed_time_parallel;
    int num_threads = omp_get_max_threads();
    double efficiency = speedup / num_threads;

    printf("Speedup: %.2f\n", speedup);
    printf("Efficiency: %.2f\n", efficiency);

    free(A);
    free(B);

    return 0;
}
