#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_SIZE 100

int main(int argc, char* argv[]) {
    int rank; // rank of process
    int num_procs; // number of processes
    char message[MESSAGE_SIZE]; // message buffer
    
    // Start up MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    MPI_Request requests[10];
    MPI_Status status[10];

    if (rank == 0) {
        for (int i = 0; i < 10; i++) {
            /* Receive */
            MPI_Irecv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i]);
            printf("%s", message);

            /* Send */
            sprintf(message, "[Process 0] Ping %d\n", i);
            MPI_Send(message, strlen(message) + 1,  MPI_CHAR, 1, i, MPI_COMM_WORLD);
        }
    }
    else if (rank == 1) {
        for (int i = 0; i < 10; i++) {
            /* Reveive */
            MPI_Irecv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i]);
            printf("%s", message);

            /* Send */
            sprintf(message, "[Process 1] Pong %d\n", i);
            MPI_Send(message, strlen(message) + 1,  MPI_CHAR, 0, i, MPI_COMM_WORLD);
        }
    }
    MPI_Waitall(10, requests, status);   
    MPI_Finalize();
    return EXIT_SUCCESS;
}
