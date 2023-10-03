#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_SIZE 100

int main(int argc, char* argv[]) {
    int rank; // rank of process
    int num_procs; // number of processes
    int destination;
    int tag = 0;
    char message[MESSAGE_SIZE]; // message buffer
    MPI_Status status;
    int buff_size;
    if (argc >= 2) {
        buff_size = atoi(argv[1]);
        printf("[Buffer size] %d\n", buff_size);
    }

    char in_messg[buff_size];
    char out_messg[buff_size];
    char* buff;
    buff = malloc(1);

    // Start up MPI
    MPI_Init(&argc, &argv);

    MPI_Buffer_attach(buff, 1);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0) {
        for (int i = 0; i < 10; i++)
        {
        /* Send */
        strcpy(out_messg, "Hello, from process 0!");
        MPI_Bsend(out_messg, strlen(out_messg) + 1,  MPI_CHAR, 1, i, MPI_COMM_WORLD);

        /* Reveive */
        MPI_Recv(in_messg, buff_size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("[Process %d, tag %d] I received:  %s\n", rank, status.MPI_TAG, in_messg);
        }
    }

    else if (rank == 1) {
        for (int i = 0; i < 10; i++)
        {
        /* Reveive */
        MPI_Recv(in_messg, buff_size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("[Process %d, tag %d] I received:  %s\n", rank, status.MPI_TAG, in_messg);

        /* Send */
        strcpy(out_messg, "Hello, from process 1!");
        MPI_Bsend(out_messg, strlen(out_messg) + 1,  MPI_CHAR, 0, i, MPI_COMM_WORLD);
        }        
    }
    
    MPI_Buffer_detach(&buff, &buff_size);
    MPI_Finalize();
    free(buff);
    return EXIT_SUCCESS;
}
