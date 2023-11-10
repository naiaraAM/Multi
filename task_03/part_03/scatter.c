#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "ERROR: Invalid number of arguments.\n");
        fprintf(stderr, "Usage: ./scatter <N>\n");
        fprintf(stderr, "N - name of file\n");
        return 1;
    }
    const char *filename = argv[1];
    int rank;
    int num_procs;
    double *data;
    FILE *file;
    double send_buff[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0)
    {
        file = fopen(filename, "r");
        if (file == NULL)
        {
            perror("ERROR: Failed to open file.\n");
            return 1;
        }
        fread(send_buff, sizeof(double), 100, file);
        fclose(file);
    }

    data = (double *)malloc((100 / num_procs) * sizeof(double));
    MPI_Scatter(send_buff, 100 / num_procs, MPI_DOUBLE, data, 100 / num_procs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < num_procs; i++)
    {
        if (i == rank)
        {
            for (int j = 0; j < 100 / num_procs; j++)
            {
                printf("%.1lf ", data[j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    free(data);
    return 0;
}
