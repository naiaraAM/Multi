#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_MESSG 10

int main(int argc, char* argv[]) {
    int rank; // rank of process
    int num_procs; // number of processes
    int source;
    int destination = 0;
    MPI_Status status;
    int index = 0;

    // get second argument from command line to set message size
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    int N = atoi(argv[1]);

    // Start up MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);


    int send_message[N];
    int recv_message[N];
    int recv_bytes;
    for (int i = 0; i < N; i++) {
        send_message[i] = i;
    }

    char* buffer = malloc(1); // 1 byte buffer memory allocation
    MPI_Buffer_attach(buffer, 1); // attach buffer to MPI

    if (rank != 0) { // multiple senders
        MPI_Bsend(send_message, N * sizeof(int), MPI_INT, destination, 0, MPI_COMM_WORLD);
        printf("[Process %d] sending message\n", rank);

    } else if (rank == 0) { // receiver 
        MPI_Recv(recv_message, N * sizeof(int), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &recv_bytes); // Get the number of received bytes
        if (recv_bytes != N * sizeof(int)) {
            fprintf(stderr, "Error in MPI_Recv. Received %d bytes instead of %lu.\n", recv_bytes, N * sizeof(int));
            MPI_Abort(MPI_COMM_WORLD, 1); // Abort the program
        }
        printf("[Process %d] received message\n", rank);

    }
    MPI_Buffer_detach(&buffer, &N);
    free(buffer);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
