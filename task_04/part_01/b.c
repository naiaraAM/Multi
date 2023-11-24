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

        for (int i = 0; i < num_procs; i++)
        {
            MPI_Send(&M_init[0][i], 1, column, i, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Recv(&M[0][rank], 1, column, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i = 0; i < num_procs; i++)
    {
        if (i == rank)
        {
            printf("Rank: %d\n", rank);
            for (int j = 0; j < N; j++)
            {
                for (int k = 0; k < N; k++)
                {
                    printf("[%.3f] ", M[j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD); // Wait for all processes to print
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
