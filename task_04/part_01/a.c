#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10

void init_matrix(double M[N][N], int dim);
void print_matrix(double M[N][N], int dim);

int main(int argc, char *argv[])
{
    int rank;
    int num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    double M_init[N][N];
    double M[N][N];
    MPI_Datatype column;
    
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            M[i][j] = 0;
        }
    }

    MPI_Type_vector(N, 1, N, MPI_DOUBLE, &column);
    MPI_Type_commit(&column);

    if (rank == 0)
    {
        printf("Initial matrix\n");
        init_matrix(M_init, N);
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("[%.3f] ", M_init[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        // Send the second column to process 1
        MPI_Send(&M_init[0][1], 1, column, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {

        // Receive the second column from process 0
        MPI_Recv(&M[0][1], 1, column, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Print the received column
        printf("Process 1 received column:\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("[%.3f] ", M[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Type_free(&column);

    MPI_Finalize();
    return 0;
}

void init_matrix(double M[N][N], int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            M[i][j] = j;
        }
    }
}
