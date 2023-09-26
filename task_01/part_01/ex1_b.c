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

    int send_buff[NUM_MESSG] ={1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int recv_buff[NUM_MESSG];
    

    // Start up MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    

    if (rank != 0) { // multiple senders
        source = rank; // change source to current rank for receiving part
        for (int tag = 0; tag < NUM_MESSG; tag++) {
            MPI_Send(&send_buff[tag], 1, MPI_INT, destination, tag, MPI_COMM_WORLD);
            printf("SENDING... Content of the message: %d\n", send_buff[tag]);
        }

    } else if (rank == 0) { // receiver 
        for (int sender_rank = 1; sender_rank < num_procs; sender_rank++) { // loop over all senders
            for (int tag = 0; tag < NUM_MESSG; tag++) {
                MPI_Recv(&recv_buff[tag], 1, MPI_INT, sender_rank, tag, MPI_COMM_WORLD, &status);
                printf("RECEIVING... Content of the message: %d from process %d\n", recv_buff[tag], sender_rank);
            }
        }
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}