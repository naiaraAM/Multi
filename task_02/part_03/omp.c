#include <stdio.h>
#include <math.h>
#include <omp.h>

/* Function to integrate */
long double f(long double x)
{
    double c = cos((double)x);
    double exponent = exp(sin((double)x));
    return (float)c * exponent;
} 

int main()  {

    long double a, b, h, x, integral;
    int n, i, tid, nthreads;

    /* Integration boundaries */
    a = 0.0;
    b = 3.14159265358979;
    /* Number of steps */
    n = 200000000;

    /* Intialization */
    h = (b-a)/n;
    integral = (f(a)+f(b))/2.0; 


    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads (2) private(i,x,tid) shared(a,b,h,n,nthreads) reduction(+:integral)
    {
        nthreads = omp_get_num_threads();
        tid = omp_get_thread_num(); 

        x = (b-a)*tid/nthreads;
        printf ("b-a = %Lf. (b-a)*tid=%Lf. (b-a)*tid/nthreads=%Lf\n", b-a, (b-a)*tid, (b-a)*tid/nthreads);
        printf ("Thead: %d. x = %Lf. nthreads = %d\n", tid, x, nthreads);
        #pragma omp for 
        for(i = 1; i <= n - 1; i++)
        {
            x = x + h;
            integral = integral + f(x);
        }
    } //End of parallel

    integral = integral * h;
    printf("[Total time] %lf\n", omp_get_wtime() - start_time);
}