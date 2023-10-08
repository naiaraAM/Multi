#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_MESSG 10

int main(int argc, char* argv[]) {
    int rank; // rank of process
    int num_procs; // number of processes
    int destination = 0;
    MPI_Status status;

    int send_buff[NUM_MESSG] ={1, 2, 3, 4, 5, 6, 7, 8, 9, 10};    

    // Start up MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    int recv_buff[NUM_MESSG * (num_procs - 1)];
    int recv_bytes; // Variable to store the number of bytes received

    if (rank != 0) { // multiple senders
        for (int tag = 0; tag < NUM_MESSG; tag++) {
            MPI_Send(&send_buff[tag], 1, MPI_INT, destination, tag, MPI_COMM_WORLD); // change number of bytes sent to 2 to see the error
            printf("SENDING... Content of the message: %d\n", send_buff[tag]);
        }

    } else if (rank == 0) { // receiver 
        for (int tag = 0; tag < NUM_MESSG * (num_procs - 1); tag++) {
            MPI_Recv(&recv_buff[tag], 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &recv_bytes); // Get the number of received bytes
            if (recv_bytes != 1) {
                fprintf(stderr, "Error in MPI_Recv. Received %d bytes instead of 1.\n", recv_bytes);
                MPI_Abort(MPI_COMM_WORLD, 1); // Abort the program
            }
            printf("RECEIVING... Content of the message: %d from source %d, Received Bytes: %d\n", recv_buff[tag], status.MPI_SOURCE, recv_bytes);
        }

    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}
