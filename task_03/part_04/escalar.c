#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define N 10

void init_vector(int *vector, int size, int rank);
void print_vector(int *vector, int size);

int *vector_1;
int *vector_2;
int num_procs;
int rank;
int rows_per_process = 0;


int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (N < num_procs)
    {
        for (int i = 0; i < N; i++)
        {
            if (i == rank)
            {
                rows_per_process = 1;
            }
        }
    } else
    {
        if (N % num_procs == 0)
        {
            rows_per_process = N / num_procs;
        }
        else
        {
            rows_per_process = N / num_procs;
            if (rank == num_procs - 1)
            {
                rows_per_process += N % num_procs;
            }
            printf("Rank: %d, rows_per_process: %d\n", rank, rows_per_process);
        }
    }
    
    vector_1 = (int *)malloc(rows_per_process * sizeof(int));
    vector_2 = (int *)malloc(rows_per_process * sizeof(int));

    init_vector(vector_1, rows_per_process, rank);
    init_vector(vector_2, rows_per_process, rank);

    int local_sum = 0;
    for (int i = 0; i < rows_per_process; i++)
    {
        local_sum += vector_1[i] * vector_2[i];
    }

    int global_sum;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("The scalar product is: %d\n", global_sum);
    }
    
    MPI_Finalize();
    free(vector_1);
    free(vector_2);
    return 0;
}

void init_vector(int *vector, int size, int rank)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = rank * size;
    }
}
void print_vector(int *vector, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", vector[i]);
    }
    printf("\n");
}