#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
  #pragma omp parallel num_threads(2)
  {
    int num1 = omp_get_thread_num();
    printf("Thread de niveau 1 : %d\n", num1);

    #pragma omp parallel num_threads(3)
    {
      int num2 = omp_get_thread_num();
      printf("  Thread de niveau 2 : %d (%d)\n", num2, num1);

      #pragma omp parallel num_threads(4)
      {
        int num3 = omp_get_thread_num();
        printf("    Thread de niveau 3 : %d (%d %d)\n", num3, num1, num2);
      }
    }
  }

  return(EXIT_SUCCESS);
}
