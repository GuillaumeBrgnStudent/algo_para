#include <stdio.h>
#include <stdlib.h>

#ifndef _OPENMP
#define omp_get_thread_num() 0
#define omp_get_num_threads() 1
#else
#include <omp.h>
#endif

int main()
{
  printf("Partie séquentielle\n");

  #pragma omp parallel
  {
    printf("Processus %d parmi %d\n", omp_get_thread_num(), omp_get_num_threads());
  }

  printf("Partie séquentielle finale.\n");

  return(EXIT_SUCCESS);
}
