#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double random_double() {
    return (double) rand() / RAND_MAX;
}

double compute_pi(int num_points, int rank, int num_procs) {
    int points_inside_circle = 0;
    int points_per_proc = num_points / num_procs;
    unsigned int seed = rank * 123456789;

    for (int i = 0; i < points_per_proc; ++i) {
        double x = random_double();
        double y = random_double();
        double distance_squared = x * x + y * y;
        if (distance_squared <= 1) {
            points_inside_circle++;
        }
    }

    int total_inside_circle = 0;
    MPI_Reduce(&points_inside_circle, &total_inside_circle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        return 4.0 * total_inside_circle / num_points;
    } else {
        return 0.0;
    }
}

int main(int argc, char *argv[]) {
    srand(12345);
    int rank, num_procs;
    long int num_points = 100000000000;
    double pi_estimate;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    start_time = MPI_Wtime();
    pi_estimate = compute_pi(num_points, rank, num_procs);
    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Приблизне значення числа пі: %f\n", pi_estimate);
        printf("Час виконання: %f секунд\n", end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}
