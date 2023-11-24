#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10

void init_matrix(float M[N][N], int dim);

int main(int argc, char *argv[])
{
    int rank;
    int num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    float M_init[N][N];
    
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            M_init[i][j] = 0;
        }
    }

    int count = N;
    int blocklengths[count];
    int displacements[count];
    MPI_Datatype triangle;
    for (int i = 0; i < N; i++)
    {
        blocklengths[i] = N - (i % N);
        displacements[i] = i * N + i;
    }
    MPI_Type_indexed(count, blocklengths, displacements, MPI_FLOAT, &triangle);
    MPI_Type_commit(&triangle);
    
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
        MPI_Send(&M_init[0][0], 1, triangle, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1)
    {
        MPI_Recv(&M_init[0][0], 1, triangle, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Received matrix\n");
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++) 
            {
                printf("[%.3f] ", M_init[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Type_free(&triangle);
    MPI_Finalize();
    return 0;
}

void init_matrix(float M[N][N], int dim)
{
    for(int i = 0; i < dim; i++)
    {
        for(int j = i; j < dim; j++)
        {
            M[i][j] = j;
        }
    }
}
