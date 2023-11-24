#include <stdio.h>
#include <mpi.h>

#define M 6 // rows
#define N 5 // columns

void init_matrix(float M_init[M][N]);

int main (int argc, char *argv[])
{
    int rank;
    int num_procs;
    float M_init[M][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    MPI_Datatype with_black_spots;
    int count = 3;
    int blocklengths[] = {1, 1, 1};
    int displacements[] = {0, 2, 4};

    MPI_Type_indexed(count, blocklengths, displacements, MPI_FLOAT, &with_black_spots);
    MPI_Type_commit(&with_black_spots);

    if (rank == 0)
    {
        printf("Initial matrix rank %d\n", rank);
        init_matrix(M_init);
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("[%.0f] ", M_init[i][j]);
            }
            printf("\n");
        }
        for (int i = 0; i < M; i++)
        {
            if (i % 2 == 0)
            {
                MPI_Send(&M_init[i][0], 1, with_black_spots, 1, 0, MPI_COMM_WORLD);
            }
        }
    } else
    {
        for (int i = 0; i < M; i++)
        {
            if (i % 2 == 0)
            {
                MPI_Recv(&M_init[i][0], 1, with_black_spots, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else if (i % 2 == 1)
            {
                for (int j = 0; j < N; j++)
                {
                    M_init[i][j] = 0.0;
                }
            }
        }
        
        printf("\nReceived matrix rank %d\n", rank);
        for(int i = 0; i < M; i++)
        {
            for(int j = 0; j < N; j++) 
            {
                printf("[%.0f] ", M_init[i][j]);
            }
            printf("\n");
        }
    }
     
    MPI_Type_free(&with_black_spots);
    MPI_Finalize();
    return 0;
}

void init_matrix(float M_init[M][N])
{
    for(int i = 0; i < M; i++)
    {
        if (i % 2 == 0)
        {
            for(int j = 0; j < N; j++)
            {
                if (j % 2 == 0)
                {
                    M_init[i][j] = 1.0;
                } else
                {
                    M_init[i][j] = 0.0;
                }
                
            }
        }
        else
        {
            for(int j = 0; j < N; j++)
            {
                M_init[i][j] = 0.0;
            }
        }
    }
}