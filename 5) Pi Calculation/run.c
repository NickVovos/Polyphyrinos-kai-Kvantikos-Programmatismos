#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double calculate_pi_sequential(long n) {
    double pi = 0.0;
    for (long i = 0; i < n; i++) {
        double term = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
        pi += term;
    }
    return 4.0 * pi;
}

double calculate_pi_parallel(long n) {
    double pi = 0.0;
    int i = 0;

    #pragma omp parallel for private(i) reduction(+:pi)
    for (i = 0; i < n; i++) {
        pi += (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);;
    }
    
    return 4.0 * pi;
}

int main() {
    long n = 10000000000;
    double start;
    double pi_sequential, pi_parallel;
    double elapsed_time, elapsed_time_parallel;


    start = omp_get_wtime();
    pi_sequential = calculate_pi_sequential(n);
    elapsed_time = omp_get_wtime() - start;
    printf("Sequential π: %.15f\n", pi_sequential);
    printf("Sequential execution time: %.6f seconds\n", elapsed_time);

    start = omp_get_wtime();
    pi_parallel = calculate_pi_parallel(n);
    elapsed_time_parallel = omp_get_wtime() - start;

    double speedup = elapsed_time / elapsed_time_parallel;
    int num_threads = omp_get_max_threads();
    double efficiency = speedup / num_threads;

    // Print results
    printf("Parallel π:   %.15f\n", pi_parallel);
    printf("Parallel execution time: %.6f seconds\n", elapsed_time_parallel);
    printf("Speedup: %.2f\n", speedup);
    printf("Efficiency: %.2f\n", efficiency);

    return 0;
}
