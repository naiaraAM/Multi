#include <stdio.h>
#include <mpi.h>


typedef struct {
    float a;
    char b;
} particle_t;

int main(void)
{
    int rank;
    int num_procs;
    MPI_Datatype type;
    MPI_Aint lb;
    MPI_Aint extent;

    MPI_Datatype particle_type_1;
    MPI_Datatype particle_type_2;
    int block_lengths[2] = {1, 1};
    MPI_Aint displacements_1[2] = {0, sizeof(float)};
    MPI_Aint displacements_2[2];
    MPI_Datatype types[2] = {MPI_FLOAT, MPI_CHAR};
    MPI_Aint base;

    int size = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    MPI_Type_create_struct(2, block_lengths, displacements_1, types, &particle_type_1);
    MPI_Type_commit(&particle_type_1);

    MPI_Get_address(&particle_type_2, &displacements_2[0]);
    MPI_Get_address(&particle_type_2, &displacements_2[1]);
    MPI_Get_address(&particle_type_2, &base);
    displacements_2[0] -= base;
    displacements_2[1] -= base;

    MPI_Type_create_struct(2, block_lengths, displacements_2, types, &particle_type_2);
    MPI_Type_commit(&particle_type_2);

    if (rank == 0)
    {
        // Double
        type = MPI_DOUBLE;
        MPI_Type_size(type, &size);
        MPI_Type_get_extent(type, &lb, &extent);
        printf("MPI_DOUBLE\n");
        printf("Size: %d, extent: %d\n\n", size, (int)extent);

        // particle with no directioning
        type = particle_type_1;
        MPI_Type_size(type, &size);
        MPI_Type_get_extent(type, &lb, &extent);
        printf("particle_type_1 (no directioning)\n");
        printf("Size: %d, extent: %d\n\n", size, (int)extent);

        // particle with directioning
        type = particle_type_2;
        MPI_Type_size(type, &size);
        MPI_Type_get_extent(type, &lb, &extent);
        printf("particle_type_2 (with directioning)\n");
        printf("Size: %d, extent: %d\n", size, (int)extent);
    }

    MPI_Type_free(&particle_type_1);
    MPI_Type_free(&particle_type_2);
    MPI_Finalize();
}