#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_MESSG 10

int main(int argc, char* argv[]) {
    int rank; // rank of process
    int num_procs; // number of processes
    int source = 0;
    int destination = 1;
    MPI_Status status;

    int send_buff[NUM_MESSG] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // initialize send buffer
    int recv_buff[NUM_MESSG];

    // Start up MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get current process id
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // get number of processes
    
    if (rank == 0) { // sender
        for (int tag = 0; tag < NUM_MESSG; tag++) {
            MPI_Send(&send_buff[tag], 1, MPI_INT, destination, tag, MPI_COMM_WORLD);
            printf("SENDING... Content of the message: %d\n", send_buff[tag]);
        }
    } else if (rank == 1) { // receiver
        for (int tag = 0; tag < NUM_MESSG; tag++) {
            MPI_Recv(&recv_buff[tag], 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
            printf("RECEIVING... Content of the message: %d\n", recv_buff[tag]);
        }        
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}