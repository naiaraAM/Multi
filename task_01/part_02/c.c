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

    if (rank == 0) { // send in desired order
        sprintf(message, "[Process 0] Message 1");
        MPI_Send(message, strlen(message) + 1,  MPI_CHAR, 1, 0, MPI_COMM_WORLD); // tag = 0
        printf("[Process 0] Sent Message 1\n");

        sprintf(message, "[Process 0] Message 2");
        MPI_Send(message, strlen(message) + 1,  MPI_CHAR, 1, 1, MPI_COMM_WORLD); // tag = 1
        printf("[Process 0] Sent Message 2\n");
    }
    else if (rank == 1) { // change order in tags, other messages waiting in buffer
        MPI_Recv(message, 100, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status); // receive message with tag = 1
        printf("[Process 1] Received: %s\n", message);

        MPI_Recv(message, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status); // receive message with tag = 0
        printf("[Process 1] Received: %s\n", message);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}
