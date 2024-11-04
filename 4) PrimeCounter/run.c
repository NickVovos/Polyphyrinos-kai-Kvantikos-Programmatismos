#include <stdio.h>
#include <omp.h>
#include <math.h>

int count_primes(int n, int chunk_size) {
    int counter = 0;
    int prime = 1;
    int d = 0;
    int x = 0;
    // Count primes using dynamic scheduling with a specific chunk size
    #pragma omp parallel for schedule(dynamic, chunk_size) private(x, prime, d)  reduction(+:counter)
    for (x = 1; x <= n; x++) {
        prime = 1;  // Assume x is prime initially
        if (x < 2) {
            prime = 0; // 0 and 1 are not prime numbers
        } else {
            for (d = 2; d <= sqrt(x); d++) {
                if (x % d == 0) {
                    prime = 0; // Not prime
                    break;
                }
            }
        }
        if (prime == 1) {
            counter++;
        }
    }

    return counter;
}

int count_primes_sequential(int n) {
    int counter = 0;

    for (int x = 1; x <= n; x++) {
        int prime = 1;
        if (x < 2) {
            prime = 0;
        } else {
            for (int d = 2; d <= sqrt(x); d++) {
                if (x % d == 0) {
                    prime = 0;
                    break;
                }
            }
        }
        if (prime == 1) {
            counter++;
        }
    }

    return counter;
}

int main() {
    int n = 90000000;

    double start_time = omp_get_wtime();
    int counter_guided_5000 = count_primes(n, 5000);
    double end_time = omp_get_wtime();
    printf("Using guided scheduling (chunk size 5000): %d primes found in %f seconds\n", counter_guided_5000, end_time - start_time);

    start_time = omp_get_wtime();
    int counter_guided_1000 = count_primes(n, 1000);
    end_time = omp_get_wtime();
    printf("Using guided scheduling (chunk size 1000): %d primes found in %f seconds\n", counter_guided_1000, end_time - start_time);

    start_time = omp_get_wtime();
    int counter_guided_500 = count_primes(n, 500);
    end_time = omp_get_wtime();
    printf("Using guided scheduling (chunk size 500): %d primes found in %f seconds\n", counter_guided_500, end_time - start_time);

    start_time = omp_get_wtime();
    int counter_guided_250 = count_primes(n, 250);
    end_time = omp_get_wtime();
    printf("Using guided scheduling (chunk size 250): %d primes found in %f seconds\n", counter_guided_250, end_time - start_time);

    start_time = omp_get_wtime();
    int counter_guided_50 = count_primes(n, 250);
    end_time = omp_get_wtime();
    printf("Using guided scheduling (chunk size 250): %d primes found in %f seconds\n", counter_guided_50, end_time - start_time);

    // Measure time for sequential execution
    // start_time = omp_get_wtime();
    // int counter_sequential = count_primes_sequential(n);
    // end_time = omp_get_wtime();
    // printf("Using sequential execution: %d primes found in %f seconds\n", counter_sequential, end_time - start_time);

    return 0;
}
