#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define T            1010 // change value of T below and over threshold

int main(int argc, char* argv[]) {
    int rank; // rank of process
    int num_procs; // number of processes
    MPI_Status status;
    int destination = 1;
    int source = 0;
    int tag = 0;

    double start_time = 0;
    double fin_time = 0;

    int* message = (int*)malloc(T * sizeof(int));

    start_time = MPI_Wtime();
    // Start up MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0) {
        printf("[Process %d] sleep(1)\n", rank);
        sleep(1);
        printf("[Process %d] sending\n", rank);
        MPI_Send(message, T, MPI_INT, destination, tag, MPI_COMM_WORLD);
        tag++;
        printf("[Process %d] sleep(6)\n", rank);
        sleep(6);
        printf("[Process %d] sending\n", rank);
        MPI_Send(message, T, MPI_INT, destination, tag, MPI_COMM_WORLD);
    }
    else if (rank == 1) {
        start_time = MPI_Wtime();
        printf("[Process %d] sleep(6)\n", rank);
        sleep(6);
        printf("[Process %d] receiving\n", rank);
        MPI_Recv(message, T, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        tag++;
        printf("[Process %d] sleep(1)\n", rank);
        sleep(1);
        printf("[Process %d] receiving\n", rank);
        MPI_Recv(message, T, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        fin_time = MPI_Wtime();
    }    
    MPI_Finalize();
    fin_time = MPI_Wtime();
    if (rank == 1) {
        printf("Execution time: %lf\n", fin_time - start_time);
    }
    free(message);
    return EXIT_SUCCESS;
}
