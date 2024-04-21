#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double random_double() {
    return (double) rand() / RAND_MAX;
}

double compute_pi(int num_points, int num_threads) {
    int points_inside_circle = 0;
    omp_set_num_threads(num_threads);

#pragma omp parallel
    {
        unsigned int seed = omp_get_thread_num() * 123456789;
#pragma omp for reduction(+:points_inside_circle)
        for (int i = 0; i < num_points; ++i) {
            double x = random_double();
            double y = random_double();
            double distance_squared = x * x + y * y;
            if (distance_squared <= 1) {
                points_inside_circle++;
            }
        }
    }
    return 4.0 * points_inside_circle / num_points;
}

int main(int argc, char *argv[]) {
    srand(12345);

    long int num_points = 100000000000;
    int num_threads = 1;

    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    double start_time = omp_get_wtime();
    double pi_estimate = compute_pi(num_points, num_threads);
    double end_time = omp_get_wtime();

    printf("Приблизне значення числа пі: %f\n", pi_estimate);
    printf("Час виконання: %f секунд\n", end_time - start_time);

    return 0;
}