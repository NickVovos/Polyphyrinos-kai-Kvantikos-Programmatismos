#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ROWS 35000
#define COLS 35000

// Function to check if A[i][j] is a local maximum
int isLocalMax(int **A, int i, int j, int w) {
    for (int di = -w; di <= w; di++) {
        for (int dj = -w; dj <= w; dj++) {
            if (di == 0 && dj == 0) continue; // Skip the center cell itself
            if (A[i + di][j + dj] >= A[i][j]) {
                return 0; // Not a local maximum
            }
        }
    }
    return 1; // Is a local maximum
}

int main() {
    // Allocate memory dynamically for A and B
    int **A = (int **)malloc(ROWS * sizeof(int *));
    int **B = (int **)malloc(ROWS * sizeof(int *));
    int **B_parallel = (int **)malloc(ROWS * sizeof(int *));
    for (int i = 0; i < ROWS; i++) {
        A[i] = (int *)malloc(COLS * sizeof(int));
        B[i] = (int *)calloc(COLS, sizeof(int)); // Initialize B with zeros
        B_parallel[i] = (int *)calloc(COLS, sizeof(int)); // Initialize B_parallel with zeros
    }

    // Seed the random number generator
    srand(time(NULL));

    // Initialize the array A with random numbers from 0 to 9
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            A[i][j] = rand() % 10; // Random number between 0 and 9
        }
    }

    int w = 1; // Window size

    // Sequential version
    double start_time = omp_get_wtime();
    for (int i = w; i < ROWS - w; i++) {
        for (int j = w; j < COLS - w; j++) {
            if (isLocalMax(A, i, j, w)) {
                B[i][j] = A[i][j]; // Store the local maximum in B
            }
        }
    }
    double elapsed_time = omp_get_wtime() - start_time;

    // Parallel version using OpenMP
    double start_time_parallel = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = w; i < ROWS - w; i++) {
        for (int j = w; j < COLS - w; j++) {
            if (isLocalMax(A, i, j, w)) {
                B_parallel[i][j] = A[i][j]; // Store the local maximum in B_parallel
            }
        }
    }
    double elapsed_time_parallel = omp_get_wtime() - start_time_parallel;

    // Calculate speedup and efficiency
    double speedup = elapsed_time / elapsed_time_parallel;
    int num_threads = omp_get_max_threads();
    double efficiency = speedup / num_threads;

    // Print results
    printf("Sequential execution time: %.6f seconds\n", elapsed_time);
    printf("Parallel execution time: %.6f seconds\n", elapsed_time_parallel);
    printf("Speedup: %.2f\n", speedup);
    printf("Efficiency: %.2f\n", efficiency);

    // Free allocated memory
    for (int i = 0; i < ROWS; i++) {
        free(A[i]);
        free(B[i]);
        free(B_parallel[i]);
    }
    free(A);
    free(B);
    free(B_parallel);

    return 0;
}
