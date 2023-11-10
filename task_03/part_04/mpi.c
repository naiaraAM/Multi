#include <stdio.h>
#include <math.h>
#include "mpi.h"

float trapez(float inicio, float fin, int rodaja, float h);
float f(float x);

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

    h = (b - a) / n;
    rodaja = n / p;
    inicio = a + yo * rodaja * h;
    fin = inicio + rodaja * h;
    integral = trapez(inicio, fin, rodaja, h);    

    MPI_Reduce(&integral, &total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);


    if (yo == 0) {
        printf("[Result]: %f\n", total);
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
