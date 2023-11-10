#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

void print_row(int *row, int dim);
void init_matrix(int **matrix, int num_rows_per_process, int C, int rank);
void init_vector(int *vector, int num_elements_vector_per_process, int rank);
void muliply_matrix_vector(int **matrix, int *vector, int num_rows_per_process, int C, int *vector_result);

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("ERROR: Invalid number of arguments.\n");
        perror("Usage: ./matrix_gather <C> <F>\n");
        perror("C - number of columns\n");
        perror("F - number of rows\n");
        exit(EXIT_FAILURE);
    }    

    int C = atoi(argv[1]);
    int F = atoi(argv[2]);
    int rank;
    int num_procs;
    int **matrix;
    int *vector;
    int *vector_result_global;
    int num_elements_vector_per_process;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0) // check if F is divisible by num_procs
    {
        if (F % num_procs != 0)
        {
            perror("ERROR: C is not divisible by the number of processes.");
            exit(1);
        }
    }

    int num_rows_per_process = F / num_procs;

    // Allocate memory for the rows
    matrix = (int**) malloc(num_rows_per_process * sizeof(int*));
    if (matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for 'matrix'.\n");
        exit(1);
    }
    for (int i = 0; i < num_rows_per_process; i++)
    {
        matrix[i] = (int*) malloc(C * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for 'matrix[%d]'.\n", i);
            exit(1);
        }
    }

    // Initialize the matrix
    init_matrix(matrix, num_rows_per_process, C, rank);

    num_elements_vector_per_process = C / num_procs;
    int vector_local[num_elements_vector_per_process];

    init_vector(vector_local, num_elements_vector_per_process, rank);
    vector = (int*) malloc(C * sizeof(int));

    for (int i = 0; i < num_procs; i++)
    {
        MPI_Gather(vector_local, num_elements_vector_per_process, MPI_INT, vector, num_elements_vector_per_process, MPI_INT, i, MPI_COMM_WORLD);
    }

    int vector_result[num_rows_per_process];
    muliply_matrix_vector(matrix, vector, num_rows_per_process, C, vector_result);

    vector_result_global = (int*) malloc(F * sizeof(int));

    for (int i = 0; i < num_procs; i++)
    {
        MPI_Gather(vector_result, num_rows_per_process, MPI_INT, vector_result_global, num_rows_per_process, MPI_INT, i, MPI_COMM_WORLD);
    }
    

    if (rank == 0)
    {
        printf("Result: ");
        print_row(vector_result_global, F);
    }

    MPI_Finalize();    

    return 0;
}

void print_row(int *row, int dim) {
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
        vector_local[i] = rank * num_elements_vector_per_process;
    }
}

void muliply_matrix_vector(int **matrix, int *vector, int num_rows_per_process, int C, int *vector_result)
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