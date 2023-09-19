#include <mpi.h>
#include <stdio.h>
int main (int argc, char **argv)
{
  int nproc; /* Numero de procesos */
  int yo; /* Mi direccion: 0<=yo<=(nproc-1) */
  puts("empieza la parte paralela");
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  printf("Numero de procesos %d, yo soy %d\n", nproc, yo);
  MPI_Finalize();
  puts("termina la parte paralela");
  return(0);
}