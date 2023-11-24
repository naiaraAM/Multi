#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define NUM_WAGONS 4
#define NUM_TICKETS 4
#define MAX_NAME_SIZE 20

typedef struct passenger {
    char name[MAX_NAME_SIZE];
    int ticket_type;
} passenger_t;

void print_wagon(passenger_t wagon[], int wagon_num);
void print_train(passenger_t train[][NUM_TICKETS]);

int main(int argc, char *argv[]) {
    int rank;
    int num_procs;

    const char *passenger_names[] = {
        "Katniss", "Peeta", "Gale", "Haymitch",
        "Effie", "Snow", "Tigris", "Plutarch",
        "Castor", "Pollux", "Cressida", "Messalla",
        "Finnick", "Brutus", "Enobaria", "Beetee"
    };

    passenger_t train[NUM_WAGONS][NUM_TICKETS];
    passenger_t local_passengers_after[NUM_TICKETS];
    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int count = 2;
    MPI_Datatype old_types[] = {MPI_CHAR, MPI_INT};
    MPI_Datatype passenger;
    MPI_Aint extent, lb;
    MPI_Type_get_extent(MPI_CHAR, &lb, &extent);
    MPI_Aint offsets[] = {0, MAX_NAME_SIZE * extent};
    int block_counts[] = {MAX_NAME_SIZE, 1};
    
    MPI_Type_create_struct(count, block_counts, offsets, old_types, &passenger);
    MPI_Type_commit(&passenger);

    MPI_Datatype wagon;
    count = NUM_WAGONS;
    int block_lengths = 1;
    int stride = 4;
    MPI_Type_vector(count, block_lengths, stride, passenger, &wagon);
    MPI_Type_commit(&wagon);


    // Check if the number of processes is correct
    if (num_procs != NUM_WAGONS) 
    {
        perror("Number of processes must be 4");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0)
    {
         // Initialize local_passengers_before
        for (int row = 0; row < NUM_WAGONS; row++)
        {
            for (int col = 0; col < NUM_TICKETS; col++)
            {
                strncpy(train[row][col].name, passenger_names[row * NUM_TICKETS + col], MAX_NAME_SIZE - 1);
                train[row][col].name[MAX_NAME_SIZE - 1] = '\0';
                train[row][col].ticket_type = col + 1;
            }
        }
        for(int i = 0; i < NUM_WAGONS; i++)
        {
            for (int j = 0; j < NUM_TICKETS; j++)
            {
                MPI_Send(&train[i][j], 1, passenger, j, 0, MPI_COMM_WORLD); // only rank 0 sends
            }
        }
    }

    for (int i = 0; i < NUM_TICKETS; i++) // receive
    {
        MPI_Recv(&local_passengers_after[i], 1, passenger, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }        
    print_wagon(local_passengers_after, rank); // each process prints its wagon
   
    MPI_Type_free(&passenger);
    MPI_Type_free(&wagon);
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

void print_train(passenger_t train[][NUM_TICKETS]) {
    for (int i = 0; i < NUM_WAGONS; i++)
    {
        print_wagon(train[i], i);
    }
    printf("\n");
}
