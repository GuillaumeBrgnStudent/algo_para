/* 
   Programme de démarrage de MPI
   Fichier à compiler avec mpicc
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int nbP, num; // Nombre de processus et numéro du processus courant

  // Initialisation du système MPI
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nbP); // Récupération du nombre de processus
  MPI_Comm_rank(MPI_COMM_WORLD, &num); // Récupération du numéro du processus courant

  // Affichage des processus MPI
  printf("Je suis le processus %d parmi %d\n", num, nbP);

  // Arrêt du système MPI
  MPI_Finalize();

  // Retour système
  return(EXIT_SUCCESS);
}
