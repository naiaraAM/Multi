#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank;
    int num_procs;
    int N;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // error management
    if (argc != 2) {
        if (rank == 0) {
            fprintf(stderr, "Not enough parameters\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    } else {
        N = atoi(argv[1]);
        if (N % (num_procs - 1) != 0) {
            fprintf(stderr, "Error: N must be multiple of number of processes - 1\n");
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }

    int local_N = N / (num_procs - 1);
    int data[N];
    for (int i = 0; i < N; i++) { // initialize data
            data[i] = i + 1;
    }

    if (rank == num_procs - 1) {
        // send initial index to slaves
        for (int dest = 0; dest < num_procs - 1; dest++) {
            int start_index = dest * local_N;
            MPI_Send(&start_index, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }

        // gather partial sums from slaves
        int total_sum = 0;
        for (int src = 0; src < num_procs - 1; src++) {
            int partial_sum;
            MPI_Recv(&partial_sum, 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += partial_sum;
        }

        printf("Addition of (1 - %d) numbers is: %d\n", N, total_sum);
    } else {
        int start_index;
        MPI_Recv(&start_index, 1, MPI_INT, num_procs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int local_sum = 0;
        for (int i = 0; i < local_N; i++) {
            local_sum += data[start_index + i];
        }

        MPI_Send(&local_sum, 1, MPI_INT, num_procs - 1, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
