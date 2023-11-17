#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define NUM_WAGONS 4
#define NUM_TICKETS 4
#define MAX_NAME_SIZE 50

typedef struct passenger {
    char name[MAX_NAME_SIZE];
    int ticket_type;
} passenger_t;

void print_wagon(passenger_t wagon[], int wagon_num);

int main(int argc, char *argv[]) {
    int rank;
    int num_procs;

    const char *passenger_names[] = {
        "Katniss", "Peeta", "Gale", "Haymitch",
        "Effie", "Snow", "Tigris", "Plutarch",
        "Castor", "Pollux", "Cressida", "Messalla",
        "Finnick", "Brutus", "Enobaria", "Beetee"
    };

    passenger_t local_passengers_before[NUM_TICKETS];
    passenger_t local_passengers_after[NUM_TICKETS];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Check if the number of processes is correct
    if (num_procs != NUM_WAGONS) 
    {
        perror("Number of processes must be 4");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Initialize local_passengers_before
    for (int j = 0; j < NUM_TICKETS; j++)
    {
        strncpy(local_passengers_before[j].name, passenger_names[rank * NUM_TICKETS + j], MAX_NAME_SIZE - 1);
        local_passengers_before[j].name[MAX_NAME_SIZE - 1] = '\0';  // Ensure null termination
        local_passengers_before[j].ticket_type = j + 1;
    }
    printf("Before\n");
    print_wagon(local_passengers_before, rank);
    MPI_Barrier(MPI_COMM_WORLD);// wait for all processes to reach this point

    MPI_Alltoall(
        local_passengers_before,
        sizeof(passenger_t),
        MPI_BYTE,
        local_passengers_after,
        sizeof(passenger_t),
        MPI_BYTE,
        MPI_COMM_WORLD
    );

    MPI_Barrier(MPI_COMM_WORLD); // wait for all processes to reach this point

    printf("After\n");
    print_wagon(local_passengers_after, rank);
    
    MPI_Finalize();
    return 0;
}

void print_wagon(passenger_t wagon[], int wagon_num) {
    printf("Wagon %d:\n", wagon_num + 1);
    for (int i = 0; i < NUM_TICKETS; i++)
    {
        printf("Ticket %d: %s\n", wagon[i].ticket_type, wagon[i].name);
    }
    printf("\n");
}
