#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {

    int rank; // rank of process
    int num_procs; // number of processes

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int size_messg = 0;

    if (argc == 2) {
        size_messg = atoi(argv[1]);
    }
    printf("size_messg = %d\n", size_messg);

    char* message = (char*)malloc(size_messg); // size of message, locked for more than 4040

    if (rank == 0) {
        for (int i = 0; i < 10; i++) {
            MPI_Send(message, size_messg, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(message, size_messg, MPI_CHAR, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("[Process 0] Ping\n");
        }
    } else if (rank == 1) {
        for (int i = 0; i < 10; i++) {
            MPI_Send(message, size_messg, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
            MPI_Recv(message, size_messg, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("[Process 1] Pong\n");
        }
    }
    MPI_Finalize();
    return 0;
}
