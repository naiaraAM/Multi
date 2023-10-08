#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_SIZE 100

int main(int argc, char* argv[]) {
    int rank; // rank of process
    int num_procs; // number of processes
    char message[MESSAGE_SIZE]; // message buffer
    MPI_Status status;

    // Start up MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0) {
        for (int i = 0; i < 10; i++) {
            /* Receive */
            MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("%s\n", message);

            /* Send */
            sprintf(message, "[Process 0] Says Ping");
            MPI_Send(message, strlen(message) + 1,  MPI_CHAR, 1, i, MPI_COMM_WORLD);
        }
    }
    else if (rank == 1) {
        for (int i = 0; i < 10; i++) {
            /* Reveive */
            MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("%s\n", message);

            /* Send */
            sprintf(message, "[Process 1] Says Pong");
            MPI_Send(message, strlen(message) + 1,  MPI_CHAR, 0, i, MPI_COMM_WORLD);
        }        
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}
