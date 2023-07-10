/*
  Exercice de parallélisation de boucle
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#define omp_get_wtime() 0
#endif

void travail(int tid)
{
  int i, j, k;
  double a = 0;

  for (i = 0 ; i < 10 ; i++){
    for (j = 0 ; j < 10 ; j++){
      for (k = 0 ; k < (tid+1) * 1000 ; k++){
        a = a + i - j;
      }
    }
  }
}

int main(int argc, char **argv)
{
  int i, opt, nbT = 4;

  // Parcours des paramètres
  while((opt = getopt(argc, argv, "t:")) != -1){
    switch(opt){
      case 't': // Nombre de threads
        nbT = atoi(optarg);
        break;
    }
  }

  #pragma omp parallel num_threads(nbT)
  {
    double deb = omp_get_wtime(), fin;
    int num = omp_get_thread_num();

    #pragma omp for 
    for(i=0; i<200; ++i){
      travail(i);
    }
    fin = omp_get_wtime();

    printf("Proc %d termine en %lfs\n", num, fin - deb);
  }

  return(EXIT_SUCCESS);
}
