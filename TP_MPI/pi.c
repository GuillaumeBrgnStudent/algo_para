/* 
   Programme de calcul du pi par intégration du quart de cercle
   Fichier à compiler avec mpicc
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

// Fonction d'intégration du cercle pour l'intervalle de x [deb : fin] avec n trapèzes
double integration(long n, double deb, double fin);

//
// Programme principal
//
int main(int argc, char **argv)
{
  int    opt;           // Variable pour les options
  double cdeb, cfin;    // Chronos de début et fin
  double pi = 0;        // Valeur finale de PI
  long    n = 10000000; // Nombre total de trapèzes

  int    nbP, num;      // Nombre de processus et numéro du processus courant
  long   nLoc;          // Nombre de trapèzes sur le processus courant
  double pi_part;       // Valeur partielle de PI
  int    reste;         // Reste du nombre de trapèzes sur le nombre de processus
  double deb, fin;      // Intervalle de x traité par le processus courant

  // Gestion des options du programme en ligne de commande  
  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
    case 'n':
      n = atol(optarg);
      break;
    }
  }

  // Initialisation du système MPI
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nbP); // Récupération du nombre de processus
  MPI_Comm_rank(MPI_COMM_WORLD, &num); // Récupération du numéro du processus courant

  // Démarrage chrono début
  cdeb = MPI_Wtime();

  // Calcul des indices et nombre de trapèzes pour le processus courant
  nLoc  =   n; // À MODIFIER ...
  reste =   0; // À MODIFIER ...
  fin   = 1.0; // À MODIFIER ...
  deb   = 0.0; // À MODIFIER ...

  // Intégration du quart de cercle dans l'intervalle de x [0.0, 1.0] avec n trapèzes
  pi_part = integration(nLoc, deb, fin); 

  // Affichage des infos de calcul de chaque processus
  printf("Le proc %d traite %ld trapèzes de %f à %f et obtient %.15lf\n", num, nLoc, deb, fin, pi_part);

  // Fusion du résultat sur le processus 0
  // ... À MODIFIER ...
  pi = pi_part;

  // Récupération chrono fin
  cfin = MPI_Wtime();

  // Affichage du résultat par le processus 0
  if(num == 0){
    printf("Valeur de pi                     : %1.15f\n", pi);
    printf("Écart à la valeur standard de pi : %.3e\n", M_PI - pi);
    printf("Calculée en                      : %fs\n", cfin - cdeb);
  }

  // Arrêt du système MPI
  MPI_Finalize();

  return(EXIT_SUCCESS);
}

//
// Fonction du cercle de rayon 1 sur l'intervalle [-1,1]
//
double cercle(double x)
{
  return sqrt(1 - x * x);
}

//
// Intégration du cercle pour l'intervalle de x [deb : fin] avec n trapèzes
//
double integration(long n, double deb, double fin)
{
  long i;                                           // Indice de boucle
  double x;                                         // valeur de x dans l'intervalle
  double h = (fin - deb) / n;                       // Largeur des trapèzes
  double somme = (cercle(deb) + cercle(fin)) / 2.0; // Contributions des deux points extrêmes

  for (i = 1; i < n; i++){ // Contributions des points internes de l'intervalle
    x = deb + h * i;
    somme += cercle(x);
  }

  return 4.0 * h * somme;
}

