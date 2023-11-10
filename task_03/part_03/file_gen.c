#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define TAM 100

int main(int argc, char *argv[])
{
   int fichero, i;
   double buffer[TAM], dato;

   fichero = open(argv[1],O_WRONLY|O_CREAT,0644);
   for (i = 0; i < TAM; i++)
   {  dato = (double)i;
      write(fichero, &dato, sizeof(double));
   }
  close(fichero);
// comprobacion
  fichero = open(argv[1],O_RDONLY);
  read(fichero, buffer, TAM*sizeof(double));
   for (i=0;i<TAM;i++)
      printf("%f  ", buffer[i]);
  close(fichero);
}
