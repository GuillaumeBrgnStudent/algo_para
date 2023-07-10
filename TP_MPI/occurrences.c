/* 
   Programme de comptabilisation des occurrences des lettres dans un texte
   Fichier à compiler avec mpicc
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>

//
// Déclaration des constantes utilisées dans le programme
//
const int NB_LETTRES = 26;

//
// Déclarations des fonctions utilisées dans le programme
//
char *genere_texte(int taille, uint graine);                                     // Génération d'un texte aléatoire
void compte_occurrences_seq(char *texte, int taille, int *occus);                // Comptage des occurrences en séquentiel
void compte_occurrences_mpi(int numP, int nbP, int tBloc, char *texte, int taille, int *occus); // Comptage des occurrences en MPI
char tabsIdem(int OccurrencesA[], int OccurrencesB[]);                           // Comparaison de deux tableaux de nombres d'occurrences

//
// Programme principal
//
int main(int argc, char **argv)
{
  //
  // Variables 
  //
  char  *texte  = NULL;         // Texte à traiter
  int    taille = 10000;        // Taille du texte à traiter
  int    tBloc  = 1000;         // Taille des blocs de texte envoyés aux travailleurs
  double deb;                   // Chrono pour les mesures de temps
  double tempsSeq = 0;          // Temps de calcul séquentiel
  double tempsMPI = 0;          // Temps de calcul MPI
  double accelMPI, effMPI;      // Accélaration et efficacité MPI
  int    opt;                   // Option de ligne de commande
  uint   graine = time(NULL);   // Graine du générateur aléatoire initialisée avec l'horloge de la machine
  int    numP, nbP;             // Numéro du processus MPI et nombre de processus MPI
  int occurrencesS[NB_LETTRES]; // Occurrences des lettres obtenues en séquentiel
  int occurrencesM[NB_LETTRES]; // Occurrences des lettres obtenues en MPI

  //
  // Lecture des paramètres
  //
  while ((opt = getopt(argc, argv, "b:g:t:")) != -1) {
    switch (opt) {
    case 'b':                 // Taille des blocs MPI
      tBloc = atoi(optarg);
      break;
    case 'g':                 // Graine
      graine = atoi(optarg);
      break;
    case 't':                 // Taille du texte
      taille = atoi(optarg);
      break;
    }
  }

  // Init MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nbP);  // Récupération du nombre de processus MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &numP); // Récupération du numéro du processus MPI courant

  //
  // Affichage des paramètres du programme
  //
  if(numP == 0){
    printf("Taille du texte      : %d\n", taille);
    printf("Graine               : %u\n", graine);
    printf("Nb Procs MPI         : %d\n", nbP);
    printf("Taille des blocs MPI : %d\n", tBloc);
  }

  //
  // Construction et initialisation aléat du texte
  // 
  srand48(graine); // Initialisation du générateur aléatoire avec la graine donnée
  
  // Génération du texte uniquement sur le processus MPI 0
  if(numP == 0){

    // Initialisation des tableaux d'occurrences 
    memset(occurrencesS, 0, NB_LETTRES * sizeof(int));
    memset(occurrencesM, 0, NB_LETTRES * sizeof(int));

    texte = genere_texte(taille, graine);
  }

  //
  // Comptage des occurrences en séquentiel sur proc MPI 0
  //
  if(numP == 0){
    deb = MPI_Wtime();
    compte_occurrences_seq(texte, taille, occurrencesS);
    tempsSeq = MPI_Wtime() - deb;
    printf("Temps Séq : %f s\n", tempsSeq);
  }

  //
  // Comptage des occurrences en MPI via un schéma maître-travailleurs piloté par le proc 0
  //
  deb = MPI_Wtime();
  compte_occurrences_mpi(numP, nbP, tBloc, texte, taille, occurrencesM);
  tempsMPI = MPI_Wtime() - deb;
  
  // Validation des résultats et affichages par le proc 0
  if(numP == 0){

    // Validation des calculs parallèles (comparaison des résultats)
    if(tabsIdem(occurrencesS, occurrencesM) == 'O'){
      printf("Les résultats séquentiel et MPI sont identiques !\n");
    }else{
      printf("Les résultats séquentiel et MPI sont DIFFÉRENTS :-/\n");
    }

    // Mesure de la qualité du parallélisme MPI
    accelMPI = tempsSeq / tempsMPI;
    effMPI = accelMPI / (nbP - 1);
    printf("MPI :\n\tTemps        : %f s\n\tAccélération : %f\n\tEfficacité   : %f\n", tempsMPI, accelMPI, effMPI);

    // Affichage des occurrences des lettres dans le texte
    printf("       Séq      MPI     Fréquences\n");
    int i;
    for(i=0; i<NB_LETTRES; ++i){
      printf("%c = %8d %8d\t%.2e\n", 'a'+i, occurrencesS[i], occurrencesM[i], (float)occurrencesS[i]/taille);
    }
  }

  // Arrêt du système MPI
  MPI_Finalize();

  //
  // Destruction des tableaux dynamiques
  //  
  free(texte);
  
  // Fin
  return 0;
}

//
// Génération du texte
//
char *genere_texte(int taille, uint graine)
{
  char *texte = malloc(taille);
  int i;

  printf("Génération du texte...");
  fflush(stdout);
  #pragma omp parallel for
  for(i=0; i<taille; ++i){
    texte[i] = (rand_r(&graine) % 95) + ' '; // 95 caractères affichables à partir de l'espace
  }
  printf("terminée.\n");
  return texte;
}

//
// Comptage des occurrences en séquentiel
//
void compte_occurrences_seq(char *texte, int taille, int *occus)
{
  int i;

  for(i=0; i<taille; ++i){
    if(texte[i] >= 'a' && texte[i] <= 'z'){
      occus[texte[i]-'a']++;
    }
    if(texte[i] >= 'A' && texte[i] <= 'Z'){
      occus[texte[i]-'A']++;
    }
  }
}

//
// Comptage des occurrences en MPI en mode client-serveur piloté par le processus 0
//
void compte_occurrences_mpi(int numP, int nbP, int tBloc, char *texte, int taille, int *occus)
{
  int i;               // Indice de boucle
  char *tampon = NULL; // Tampon mémoire de travail
  MPI_Status etat;     // État de réception

  // Allocation du tampon local
  tampon = (char *) malloc(tBloc);
  
  // Gestion du maître (proc 0)
  if(numP == 0){
  
    int nbRestants = taille; // Nombre de caractères restant à traiter dans le texte
    int src;                 // Source du message reçu
    int nbEnvoyes;           // Nombre de caractères envoyés
    int occs[NB_LETTRES];    // Nombre d'occurrences
    int nbAttendus = 0;      // Nombre de résultats attendus (tâches en cours)

    // 1ère distribution du maître (proc 0)
    for(i=1; i<nbP; ++i){

      // Calcul du nombre de caractères à envoyer
      // ...

      // Copie dans tampon et envoi au processus cible
      // ...

    }

    // Boucle de gestion du travail... tant qu'il en reste
    while(nbAttendus > 0){

      // Récupération d'un résultat (réception d'un message d'un autre processus)
      // ...
      
      // Mise à jour du résultat global
      // ...

      // Envoi du bloc suivant au processus libre s'il reste du travail
      // ...

    }

    // Signal de fin (message vide) envoyé à chaque travailleur
    // ...

  }else{ // Gestion des travailleurs

    int oloc[NB_LETTRES]; // Tableau local des occurrences des lettres
    int nbRecus = 0;      // Nombre de caractères reçus
    char fini = 'N';      // Indicateur de fin du travail

    while(fini == 'N'){

      // Attente d'un message du maître
      // ...

      // Récupération de la taille du message
      // ...
      
      // Distinction entre message de texte et message de fin (vide)
      if(nbRecus == 0){ // Taille nulle indique la fin du travail

        // Arrêt du travail
        fini = 'O';
        
      }else{ // Taille non nulle indique un morceau de texte à traiter

        // Réception du texte
        // ...

        // Initialisation des nombres d'occurrences locaux
        // ...
        
        // Traitement du texte
        // ...

        // Envoi du résultat au maître
        // ...

      }
    }
  }

  // Libération du tampon local
  free(tampon);
}

//
// Comparaison de deux tableaux d'occurrences de lettres
//
char tabsIdem(int OccurrencesA[], int OccurrencesB[])
{
  int i;

  for(i=0; i<NB_LETTRES && OccurrencesA[i] == OccurrencesB[i]; ++i);
  
  if(i < NB_LETTRES){
    return 'N';
  }else{
    return 'O';
  }
}
