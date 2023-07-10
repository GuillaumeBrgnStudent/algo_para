//
// Programme de comptabilisation des occurrences des lettres dans un texte
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

//
// Déclaration des constantes utilisées dans le programme
//
const int NB_LETTRES = 26;

//
// Déclarations des fonctions utilisées dans le programme
//
void aide(char *nom);                                                            // Aide en ligne du programme
char *genere_texte(int taille, uint graine);                                     // Génération d'un texte aléatoire
void compte_occurrences_seq(char *texte, int taille, int *occus);                // Comptage des occurrences en séquentiel
void compte_occurrences_omp(int nbThreads, char *texte, int taille, int *occus); // Comptage des occurrences en OpenMP
char tabsIdem(int OccurrencesA[], int OccurrencesB[]);                           // Comparaison de deux tableaux de nombres d'occurrences
void afficheOccurrences(int *occSeq, int *occPar);                               // Affichage des occurrences

//
// Programme principal
//
int main(int argc, char **argv)
{
  //
  // Variables 
  //
  char *texte = NULL;           // Texte à traiter
  int taille = 10000;           // Taille du texte à traiter
  double deb;                   // Chrono pour les mesures de temps
  double tempsSeq = 0;          // Temps de calcul séquentiel
  double tempsOMP = 0;          // Temps de calcul OpenMP
  double accelOMP, effOMP;      // Accélaration et efficacité OpenMP
  int opt;                      // Option de ligne de commande
  uint graine = time(NULL);     // Graine du générateur aléatoire initialisée avec l'horloge de la machine
  int nbThreads = 2;            // Nombre de threads utilisés
  int occurrencesS[NB_LETTRES]; // Occurrences des lettres obtenues en séquentiel
  int occurrencesO[NB_LETTRES]; // Occurrences des lettres obtenues en OpenMP

  //
  // Lecture des paramètres
  //
  while ((opt = getopt(argc, argv, "g:hp:t:")) != -1) {
    switch (opt) {
    case 'g':                 // Graine
      graine = atoi(optarg);
      break;
    case 'h':                 // Graine
      aide(argv[0]);
      break;
    case 'p':                 // Nombre de threads OpenMP
      nbThreads = atoi(optarg);
      break;
    case 't':                 // Taille du texte
      taille = atoi(optarg);
      break;
    }
  }

  //
  // Affichage des paramètres du programme
  //
  printf("Taille du texte      : %d\n", taille);
  printf("Graine               : %u\n", graine);
  printf("Nb Threads OMP       : %d\n", nbThreads);

  //
  // Construction et initialisation aléat du texte
  // 
  srand48(graine);                      // Initialisation du générateur aléatoire avec la graine donnée
  texte = genere_texte(taille, graine); // Génération pseudo-aléatoire du texte
  
  // Initialisation des tableaux d'occurrences 
  memset(occurrencesS, 0, NB_LETTRES * sizeof(int));
  memset(occurrencesO, 0, NB_LETTRES * sizeof(int));

  //
  // Comptage des occurrences en séquentiel
  //
  deb = omp_get_wtime();
  compte_occurrences_seq(texte, taille, occurrencesS);
  tempsSeq = omp_get_wtime() - deb;
  printf("Temps Séq : %f s\n", tempsSeq);

  //
  // Comptage des occurrences en OpenMP
  //
  deb = omp_get_wtime();
  compte_occurrences_omp(nbThreads, texte, taille, occurrencesO);
  tempsOMP = omp_get_wtime() - deb;

  //
  // Validation des calculs parallèles (comparaison des résultats)
  //
  if(tabsIdem(occurrencesS, occurrencesO) == 'O'){
    printf("Les résultats séquentiel et OpenMP sont identiques !\n");
  }else{
    printf("Les résultats séquentiel et OpenMP sont DIFFÉRENTS :-/\n");
  }
  afficheOccurrences(occurrencesS, occurrencesO);

  // Mesure de la qualité du parallélisme OpenMP
  accelOMP = tempsSeq / tempsOMP;
  effOMP = accelOMP / nbThreads;
  printf("OpenMP :\n\tTemps        : %f s\n\tAccélération : %f\n\tEfficacité   : %f\n", tempsOMP, accelOMP, effOMP);

  //
  // Destruction du tableau dynamique
  //  
  free(texte);
  
  // Fin
  return (EXIT_SUCCESS);
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

  memset(occus, 0, NB_LETTRES * sizeof(int));

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
// Comptage des occurrences en OpenMP
//
void compte_occurrences_omp(int nbThreads, char *texte, int taille, int *occus)
{
  int i;

  //
  // À COMPLÉTER/MODIFIER !
  //
  memset(occus, 0, NB_LETTRES * sizeof(int));
    
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

//
// Affichage  des occurrences des lettres dans le texte
//
void afficheOccurrences(int *occSeq, int *occPar)
{
  int i;

  printf("       Séq      Par     Fréquences\n");
  for(i=0; i<NB_LETTRES; ++i){
    printf("%c = %8d %8d\n", 'a'+i, occSeq[i], occPar[i]);
  }
}

//
// Aide en ligne
//
void aide(char *nom)
{
  printf("Usage : %s <-g int> <-h> <-p int> <-t int>\n", nom);
  printf("\t-g int : graine du générateur aléatoire\n");
  printf("\t-h     : aide en ligne\n");
  printf("\t-p int : nombre de processus OpenMP\n");
  printf("\t-t int : taille du texte généré\n");
  exit(EXIT_SUCCESS);

}

