#include <stdio.h>
#include <mpi.h>

typedef struct {
    float a;
    char b;
} particle_t;

int main(void) {
    int rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype particle_type;
    int block_lengths[2] = {1, 1};
    MPI_Aint displacements[2] = {0, sizeof(float)};
    MPI_Datatype types[2] = {MPI_FLOAT, MPI_CHAR};

    MPI_Type_create_struct(2, block_lengths, displacements, types, &particle_type);
    MPI_Type_commit(&particle_type);

    if (rank == 0) {
        particle_t particle;
        particle.a = 1.0;
        particle.b = 'a';
        MPI_Send(&particle, 1, particle_type, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        particle_t particle;
        MPI_Status status;

        MPI_Recv(&particle, 1, particle_type, 0, 0, MPI_COMM_WORLD, &status);

        int count;
        MPI_Get_elements(&status, particle_type, &count); // gets the number of elements received
        printf("Count MPI_Get_elements: %d\n", count);

        MPI_Get_count(&status, particle_type, &count); // gets the number of elements received of type particle_type
        printf("Count MPI_Get_count: %d\n", count);
    }

    MPI_Type_free(&particle_type);
    MPI_Finalize();

    return 0;
}
