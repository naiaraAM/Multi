#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void print_row(int *row, int dim);
void init_matrix(int **matrix, int num_rows_per_process, int C, int rank);
void init_vector(int *vector_local, int num_elements_vector_per_process, int rank);
void multiply_matrix_vector(int **matrix, int *vector, int num_rows_per_process, int C, int *vector_result);
void prepare_vector(int *vector, int num_elements_vector_per_process, int num_procs);


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "ERROR: Invalid number of arguments.\n");
        fprintf(stderr, "Usage: ./matrix_gather <C> <F>\n");
        fprintf(stderr, "C - number of columns\n");
        fprintf(stderr, "F - number of rows\n");
        exit(EXIT_FAILURE);
    }

    int C = atoi(argv[1]);
    int F = atoi(argv[2]);
    int rank;
    int num_procs;
    int **matrix;
    int *partial_vector;
    int *vector;
    int *global_result;
    int num_elements_vector_per_process;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (F % num_procs != 0)
    {
        fprintf(stderr, "ERROR: F is not divisible by the number of processes.\n");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    int num_rows_per_process = F / num_procs;

    // Allocate memory for the rows
    matrix = (int **)malloc(num_rows_per_process * sizeof(int *));
    if (matrix == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for 'matrix'.\n");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_rows_per_process; i++)
    {
        matrix[i] = (int *)malloc(C * sizeof(int));
        if (matrix[i] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for 'matrix[%d]'.\n", i);
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }
    }

    // Initialize the matrix
    init_matrix(matrix, num_rows_per_process, C, rank);    

    num_elements_vector_per_process = C / num_procs;
    partial_vector = (int *)malloc(C * sizeof(int));
    vector = (int *)malloc(C * sizeof(int));

    // Initialize the vector
    init_vector(partial_vector, C, rank);

    // Share partial result
    MPI_Alltoall(
        partial_vector,
        num_elements_vector_per_process,
        MPI_INT,
        vector,
        num_elements_vector_per_process,
        MPI_INT,
        MPI_COMM_WORLD
    );

    int partial_result[F]; // repeat result for each process
    

    multiply_matrix_vector(matrix, vector, num_rows_per_process, C, partial_result);
    prepare_vector(partial_result, num_rows_per_process, num_procs); // repeat result for each process

    global_result = (int *)malloc(F * sizeof(int));
    MPI_Alltoall(
        partial_result,
        num_rows_per_process,
        MPI_INT,
        global_result,
        num_rows_per_process,
        MPI_INT,
        MPI_COMM_WORLD
    );

    for (int i = 0; i < num_procs; i++)
    {
        if (i == rank)
        {
            printf("Rank: %d\n", rank);
            print_row(global_result, F);
        }
        
    }

    free(partial_vector);
    free(vector);
    for (int i = 0; i < num_rows_per_process; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    MPI_Finalize();

    return 0;
}

void print_row(int *row, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        printf("%d ", row[i]);
    }
    printf("\n");
}

void init_matrix(int **matrix, int num_rows_per_process, int C, int rank)
{
    for (int i = 0; i < num_rows_per_process; i++)
    {
        for (int j = 0; j < C; j++)
        {
            matrix[i][j] = rank * num_rows_per_process + i;
        }
    }
}

void init_vector(int *vector_local, int num_elements_vector_per_process, int rank)
{
    for (int i = 0; i < num_elements_vector_per_process; i++)
    {
        vector_local[i] = rank ;
    }
}

void multiply_matrix_vector(int **matrix, int *vector, int num_rows_per_process, int C, int *vector_result)
{    
    for (int i = 0; i < num_rows_per_process; i++)
    {
        int partial_sum = 0;
        for (int j = 0; j < C; j++)
        {
            partial_sum += matrix[i][j] * vector[j];
        }
        vector_result[i] = partial_sum;
    }
}

void prepare_vector(int *vector, int num_elements_vector_per_process, int num_procs)
{
    for (int i = 0; i < num_procs; i++)
    {
        for (int j = 0; j < num_elements_vector_per_process; j++)
        {
            vector[i * num_elements_vector_per_process + j] = vector[j];
        }   
    }  
}