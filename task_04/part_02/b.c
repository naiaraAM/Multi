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
    MPI_Aint displacements[2];
    MPI_Datatype types[2] = {MPI_FLOAT, MPI_CHAR};

    particle_t particle;
    MPI_Get_address(&particle.a, &displacements[0]);
    MPI_Get_address(&particle.b, &displacements[1]);
    MPI_Aint base;
    MPI_Get_address(&particle, &base);

    displacements[0] -= base;
    displacements[1] -= base;

    MPI_Type_create_struct(2, block_lengths, displacements, types, &particle_type);
    MPI_Type_commit(&particle_type);

    if (rank == 0) {
        particle.a = 1.0;
        particle.b = 'a';
        MPI_Send(&particle, 1, particle_type, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Recv(&particle, 1, particle_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Particle a: %.2f\n", particle.a);
        printf("Particle b: %c\n", particle.b);
    }

    MPI_Type_free(&particle_type);
    MPI_Finalize();

    return 0;
}
