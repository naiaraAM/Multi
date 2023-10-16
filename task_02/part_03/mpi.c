#include <stdio.h>
#include <math.h>

#include "mpi.h"

int main(int argc, char** argv)  
{
    float a = 0.0, b = 10.0, h, x, integral;
    int n = 1000, i;
    int yo, p, origen, rodaja, destino = 0, tag = 0;
    float inicio, fin;
    float total = 0.0;
    MPI_Status status;
    double start_time;
    double fin_time;

    float trapez(float inicio, float fin, int rodaja, float h);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &yo);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (yo == 0) {
        printf("Introduce %f , %f, %d\n", a, b, n);
        scanf("%f %f %d", &a, &b, &n);
    }

    // Broadcast los valores de a, b y n desde el proceso 0 a todos los demás.
    MPI_Bcast(&a, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD); // Espera a que todos los procesos hayan recibido los valores.

    h = (b - a) / n;
    rodaja = n / p;
    inicio = a + yo * rodaja * h;
    fin = inicio + rodaja * h;
    start_time = MPI_Wtime();
    integral = trapez(inicio, fin, rodaja, h);
    fin_time = MPI_Wtime();

    //printf("Soy %d inicio y fin son %f - %f integral ini es %f h %f\n", yo, inicio, fin, integral, h);
    

    if (yo == 0) {
        total = integral;
        for(origen = 1; origen < p; origen++) {
        MPI_Recv(&integral, 1, MPI_FLOAT, origen, tag, MPI_COMM_WORLD, &status);
        total += integral;
        }
        //printf("Con %d trapezoides la estimacion es de %f\n", n, total);
        printf("\n[Total time] %lf\n\n", fin_time - start_time);
        printf("[Deviation] %lf\n", fabs(total));
    } else {
        MPI_Send(&integral, 1, MPI_FLOAT, destino, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}

/* definición de la función */
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
