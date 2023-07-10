#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void travail(int tid)
{
  int i, j, k;
  double a = 0, tps;
  int num = omp_get_thread_num();

  tps = omp_get_wtime();
  for (i = 0 ; i < 200 ; i++){
    for (j = 0 ; j < 100 ; j++){
      for (k = 0 ; k < (tid+1) * 1000 ; k++){
        a = a + i - j;
      }
    }
  }
  tps = omp_get_wtime() - tps;
  printf("Thread %d calcule le travail %d en %fs\n", num, tid, tps);
}

int main()
{
  double deb, fin, duree;

  deb = omp_get_wtime();
  #pragma omp parallel num_threads(4)
  {
    int i;

    for(i=0; i<20; ++i){
      travail(i);
    }
  }
  fin = omp_get_wtime();
  duree = fin - deb;

  printf("Calcul terminé en %lfs\n", duree);

  return(EXIT_SUCCESS);
}
