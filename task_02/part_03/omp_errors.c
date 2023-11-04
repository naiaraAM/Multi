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

    omp_lock_t lock;
    omp_init_lock(&lock);

    a = 0.0;
    b = 3.14159265358979;
    
    n = 200000000;

    h = (b - a) / n;
    integral = (f(a) + f(b)) / 2.0; 

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads (2) private(i,x,tid) shared(a,b,h,n,nthreads,integral)
    {
        nthreads = omp_get_num_threads();
        tid = omp_get_thread_num(); 

        x = (b - a) * tid / nthreads;

        #pragma omp for 
        for (i = 1; i <= n - 1; i++)
        {
            x = x + h;
            omp_set_lock(&lock);
            integral = integral + f(x);
            omp_unset_lock(&lock);
        }
    } //End of parallel
    integral = integral * h;
    printf("[Total time] %lf\n", omp_get_wtime() - start_time);
    omp_destroy_lock(&lock);
}

