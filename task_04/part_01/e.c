#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define N 8

void init_matrix(double M[N][N]);

int main(void)
{
    int rank;
    int num_procs;
    double M[N][N];
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int count = N * 2; // include the white and black squares
    int blocklength[] = {1, 1, 1, 1, 1, 1, 1, 1};
    int displacements[] = {1, 3, 5, 7, 8, 10, 12, 14};
    MPI_Datatype row_blocks;
    MPI_Type_indexed(count, blocklength, displacements, MPI_DOUBLE, &row_blocks);
    MPI_Type_commit(&row_blocks);

    count = N / 2;
    MPI_Datatype matrix;
    MPI_Type_contiguous(count, row_blocks, &matrix);
    MPI_Type_commit(&matrix);
    

    if (rank == 0)
    {
        printf("Init matrix\n");
        init_matrix(M);
        MPI_Send(&M[0][0], 1, matrix, 1, 0, MPI_COMM_WORLD);
    }
    if (rank == 1)
    {
        MPI_Recv(&M[0][0], 1, matrix, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("Received matrix\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("[%1.01f] ", fabs(M[i][j]));
            }
            printf("\n");
        }
    }

    MPI_Type_free(&row_blocks);
    MPI_Type_free(&matrix);
    MPI_Finalize();
}

void init_matrix(double M[N][N])
{
    for (int i = 0; i < N; i++)
    {
        if (i % 2 == 0) // white starts
        {
            for (int j = 0; j < N; j++)
            {
                if (j % 2 == 0)
                {
                    M[i][j] = 0.0;
                    printf("[%1.1f] ", M[i][j]);
                }
                else
                {
                    M[i][j] = 1.0;
                    printf("[%1.1f] ", M[i][j]);
                }
            }
        }
        else
        {
            for (int j = 0; j < N; j++)
            {
                if (j % 2 == 0)
                {
                    M[i][j] = 1.0;
                    printf("[%1.1f] ", M[i][j]);
                }
                else
                {
                    M[i][j] = 0.0;
                    printf("[%1.1f] ", M[i][j]);
                }
            }
        }
        printf("\n");
    }
}
