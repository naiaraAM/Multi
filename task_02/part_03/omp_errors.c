#include <stdio.h>
#include <math.h>
#include <omp.h>


/* Function to integrate */
long double f(long double x)
  { return sin(x); } 


int main()  {

  long double a, b, h, x, integral;
  int n, i, tid, nthreads;

  omp_lock_t lock;
  omp_init_lock(&lock);



  /* Integration boundaries */
  a = 0.0;
  b = 3.14159265358979;
//b = 4;
  /* Number of steps */
  n = 200000000;

  /* Intialization */
  h = (b-a)/n;
  integral = (f(a)+f(b))/2.0; 

//  x = a;

double start_time = omp_get_wtime();

#pragma omp parallel num_threads (4) private(i,x,tid) shared(a,b,h,n,nthreads,integral)
{
  nthreads = omp_get_num_threads();
  tid = omp_get_thread_num(); 

  x = (b-a)*tid/nthreads;

  #pragma omp for 
  for(i=1; i<=n-1; i++)
  {
    x = x + h;
//  #pragma omp critical (integral)
//  {
//#pragma omp atomic
omp_set_lock(&lock);
    integral = integral + f(x);
omp_unset_lock(&lock);
//  }
  }
} //End of parallel
  integral = integral * h;
  //printf("With %d steps, the result is %Lf\n", n, integral);
  printf("[Total time] %lf\n", omp_get_wtime() - start_time);
omp_destroy_lock(&lock);
}

