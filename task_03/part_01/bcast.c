#include <stdio.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

float f(float x);
float trapez(float inicio, float fin, int rodaja, float h);

int main(int argc, char* argv[])  {
    float a, b, h, x, integral;
    int n, i;

    int rank;
    int num_procs;
    int root = 0;
    int origen;
    int rodaja;
    int tag = 0;
    float total = 0.0;
    float inicio, fin;
    double start_time, fin_time;


    float buff[3];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get current process id
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // get number of processes

    if (rank == root) // process in charge of reading input data
    {
        scanf("%f %f %d", &a, &b, &n);
        buff[0] = a;
        buff[1] = b;
        buff[2] = n;
        MPI_Bcast(buff, 3, MPI_FLOAT, root, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(buff, 3, MPI_FLOAT, root, MPI_COMM_WORLD);
        a = buff[0];
        b = buff[1];
        n = buff[2];
    }

    h = (b - a) / n;
    rodaja = n / num_procs;
    inicio = a + rank * rodaja * h;
    fin = inicio + rodaja * h;
    start_time = MPI_Wtime();
    integral = trapez(inicio, fin, rodaja, h);
    fin_time = MPI_Wtime();

    if (rank == root)
    {

        total = integral;
        for (origen = 1; origen < num_procs; origen++) {
        MPI_Recv(&integral, 1, MPI_FLOAT, origen, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total += integral;
        }
        printf("Con %d trapezoides la estimacion es de %f\n", n, total);
        printf("\n[Total time] %lf\n\n", fin_time - start_time);
    } else 
    {
        MPI_Send(&integral, 1, MPI_FLOAT, root, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}

float f(float x)
{
    double c = cos((double)x);
    double exponent = exp(sin((double)x));
    return (float)c * exponent;
}
float trapez(float inicio, float fin, int rodaja, float h)
{
    float integral, x, f(float x);
    int i;
    integral = (f(inicio) + f(fin)) / 2.0;
    for(x = inicio, i = 1; i <= rodaja - 1; i++)
    {
        x += h;
        integral += f(x);
    }
    integral = integral * h;
    return integral;
}
