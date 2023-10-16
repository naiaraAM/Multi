#include <pthread.h>
#include <stdio.h>
#include <math.h>

#define  P  2

int n, rodaja;
float a, b, h, total = 0.0;

typedef struct {
    float inicio;
    float fin;
    float integral;
} trap;

/* definicion de la funcion */
float f(float x);

void * trapez (void * argu)
{
    float x;
    int i;
    printf("rodaja y h  %d - %f \n", rodaja, h);
    printf("inicio y fin %f - %f \n", ((trap*)argu)->inicio, ((trap*)argu)->fin);

    ((trap*)argu)->integral = (f(((trap*)argu)->inicio) + f(((trap*)argu)->fin)) / 2.0;  

    printf("total  %f \n",  ((trap*)argu)->integral);

    for(x =((trap*)argu)->inicio, i=1; i<=rodaja-1; i++) 
    {
            x += h;
            ((trap*)argu)->integral += f(x);
    }
    ((trap*)argu)->integral = ((trap*)argu)->integral * h;

    printf("total  %f \n",  ((trap*)argu)->integral);

    }

    int main()
    {
    pthread_t  th[P];
    trap       tra[P];
    void       *st[P];
    int i;

    // entrada de datos
    printf("Límite inferior: ");
    scanf("%f", &a);
    printf("Límite superior: ");
    scanf("%f", &b);
    printf("Número de trapezoides: ");
    scanf("%d", &n);

    h = (b - a) / n;
    rodaja = n / P;

    for(i = 0; i < P; i++) {
        tra[i].inicio = a + i * rodaja * h;
        tra[i].fin = tra[i].inicio + rodaja * h;
    //  printf("inicio y fin de %d son %f - %f \n", i, tra[i].inicio, tra[i].fin);
    }
    for(i = 0; i < P; i++)
        if (pthread_create(&th[i], NULL, trapez, &tra[i]) != 0)
        perror("error del thread uno");
    for(i = 0; i < P; i++)
        if (pthread_join(th[i], (void **)&st[i]) != 0)
        perror("join error 1");
    for(i = 0; i < P; i++)
        total += tra[i].integral;
    printf("Con %d trapezoides la estimacion es de %f\n", n, total);
}

float f(float x) {
    double c = cos((double)x);
    double exponent = exp(sin((double)x));
    return (float)c * exponent;
}