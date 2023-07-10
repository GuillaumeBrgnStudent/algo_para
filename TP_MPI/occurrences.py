#!/bin/python3
# -*- coding: utf-8 -*-

# Programme qui compte les occurrences des lettres dans un texte
# Execution : invocation directe dans un terminal ou via python3

# Bibliothèques utilisées
import sys
import math
import random as rnd
import string

# Import de MPI
from mpi4py import MPI

#
# Constantes utilisées dans le programme
#
NB_LETTRES = 26

#
# Génération pseudo-aléatoire d'un texte de n caractères
#
def genereTexte(n, graine):
    texte = ""

    # Init PRNG
    rnd.seed(graine)
    # Ensemble des caractères affichables (lettres, ponctuations,...)
    taille = len(string.printable)
    
    # Tirage aléatoire de chaque caractère dans l'ensemble affichable
    for i in range(n):
        texte += string.printable[rnd.randint(0,taille-1)]

    return texte

#
# Version séquentielle du comptage des occurrences
#
def compte_occs_seq(texte, n):
    # Tableau des occurrences
    occus = [0 for i in range(NB_LETTRES)]
    # Parcours du texte
    for car in texte:
        if (car in string.ascii_lowercase):
            occus[ord(car) - ord('a')] += 1
        elif (car in string.ascii_uppercase):
            occus[ord(car) - ord('A')] += 1

    return occus

#
# Version parallèle du comptage des lettres avec un schéma maître-travailleurs
# !! CETTE FONCTION DOIT ÊTRE MODIFIÉE !!
#
def compte_occs_par(texte, n, comm, idP, nbP, block):
    # Tableau des occurrences
    occus = [0 for i in range(NB_LETTRES)]
    # Identifiant du maître
    maitre = 0

    # Processus maître
    if (idP == maitre):
        
        # Variables locales
        reste      = n # Nombre de caractères restant à traiter
        position   = 0 # Position du 1er caractère du sous-texte
        nbAEnvoyer = 0 # Nombre de caractères à envoyer
        nbTaches   = 0 # Nombre de tâches actives

        # 1ère distribution des blocs de texte (1 pour chaque travailleur)
        for i in range(1,nbP):
            
            # Nombre de caractères à envoyer au travailleur i
            # ...

            # Envoi du bloc au travailleur i avec l'étiquette 0
            # ...

            # Mise à jour des informations globales
            # ...

            # Affichage des informations (DOIT ÊTRE COMMENTÉ PENDANT LES MESURES DE PERFS)
            print("Envoi d'un bloc de taille %d au processus %d" % (nbAEnvoyer, i))

        # Boucle de réception des résultats et envoi d'autres travaux tant qu'il en reste
        while (nbTaches > 0):

            # Variable contenant les infos d'état de la réception
            info = MPI.Status()

            # Attente d'un résultat de n'importe quel travailleur
            # ...
            
            # Mise à jour du nombre de tâches en cours
            # ...
            
            # Mise à jour du résultat global
            # ...

            # Envoi du bloc de texte suivant s'il en reste
            if (reste > 0):
               pass # LIGNE À SUPPRIMER LORSQUE LES LIGNES CI-DESSOUS SONT COMPLÉTÉES

               # Nombre de caractères à envoyer au travailleur i
               # ...

               # Envoi du block au MÊME travailleur avec l'étiquette 0
               # ...

               # Affichage des informations (DOIT ÊTRE COMMENTÉ PENDANT LES MESURES DE PERFS)
               # print("Envoi d'un bloc de taille %d au processus %d" % (nbAEnvoyer, ...))

               # Mise à jour des infos globales
               # ...

        # Tout le texte a été traité
        # Envoi du signal de terminaison aux travailleurs
        # ...
        
          #
    else: # Travailleurs
          #

        # Variables locales
        fini    = False
        nbRecus = 0

        # Boucle de travail
        while (not fini):
            
            # Attente d'un travail du maître avec l'étiquette 0
            # ...

            # Nombre de caractères reçus
            # ...

            # Un message vide correspond au signal de termination 
            if (nbRecus == 0):
                # Indique la fin du travail
                fini = True
            else:
                pass # LIGNE À SUPPRIMER LORSQUE LES LIGNES CI-DESSOUS SONT COMPLÉTÉES

                # Comptage des occurrences par le travailleur courant
                # ...

                # Envoi du résultat au maître avec l'étiquette 1
                # ...

    return occus

#
# Comparaison des contenus des tableaux de résultats : retourne True si identiques
#
def compare_tableaux(arrayA, arrayB):
    if (len(arrayA) != len(arrayB)):
        return False
    for i in range(len(arrayA)):
        if (arrayA[i] != arrayB[i]):
            return False
    return True

#
# Fonction principale
#
def main():

    # Valeurs par défaut des paramètres du programme
    n      = 1000000 # Taille du texte
    bloc   = 1000    # Taille d'un bloc
    graine = 1       # Graine du PRNG

    # Paramètres de la ligne de commande
    argc = len(sys.argv)
    for i in range(argc):
        if sys.argv[i] == "-b":
            i = i + 1
            bloc = int(sys.argv[i])
        elif sys.argv[i] == "-t":
            i = i + 1
            n = int(sys.argv[i])
        elif sys.argv[i] == "-g":
            i = i + 1
            graine = int(sys.argv[i])
    
    # Variables internes du programme
    texte      = None # Texte
    tdeb       = 0.0  # Temps de début
    tfin       = 0.0  # Temps de fin
    tSeq       = 0.0  # Temps séquentiel
    tPar       = 0.0  # Temps parallèle
    accel      = 0.0  # Accélération
    efficacite = 0.0  # Efficacité

    # Init MPI
    comm = MPI.COMM_WORLD  # Groupe de communication
    idP  = comm.Get_rank() # Identifiant du processus courant
    nbP  = comm.Get_size() # Nombre de processus dans le groupe

    # Affichage des paramètres du programme par le processus 0 (maître)
    if (idP == 0):
        print()
        print("Taille texte : %d" % (n))
        print("Taille blocs : %d" % (bloc))
        print("Graine       : %u" % (graine))
        print("Nb processus : %d" % (nbP))
        print()

    # Génération du texte par le processus 0 (maître)
    if(idP == 0):
        texte = genereTexte(n, graine)
    
    #
    # Version SÉQUENTIELLE du comptage des occurrences par le processus 0 (maître)
    #
    if (idP == 0):
        # Temps début
        tdeb = MPI.Wtime()
        # Comptage séquentiel
        occsSeq = compte_occs_seq(texte, n)
        # Temps fin
        tfin = MPI.Wtime()
        # Temps total
        tSeq = tfin - tdeb

    #
    # Version PARALLÈLE du comptage des occurrences avec un schéma Maître-Travailleurs (maître = 0)
    #
    # Temps début
    tdeb = MPI.Wtime()
    # Comptage séquentiel
    occsPar = compte_occs_par(texte, n, comm, idP, nbP, bloc)
    # Temps fin
    tfin = MPI.Wtime()
    # Temps total
    tPar = tfin - tdeb

    # Validation des résultats et affichage des informations par le maître
    if (idP == 0):
        print()

        # Validation (comparaison)       
        if (compare_tableaux(occsSeq, occsPar)):
            print("Résultats IDENTIQUES entre les versions séquentielle et parallèle !")
        else:
            print("Résultats DIFFÉRENTS entre les versions séquentielle et parallèle !")
        print()

        # Affichage des occurrences des lettres dans le texte
        print("       Seq      Par     Fréquence");
        for i in range(NB_LETTRES):
            print("%c = %8d %8d    %.2e" % (ord('a')+i, occsSeq[i], occsPar[i], occsSeq[i]/n))
        print()

        # Évaluation des performances
        accel      = tSeq / tPar
        efficacite = accel / (nbP - 1)
        print("Temps séquentiel : %f s\nTemps parallèle  : %f s\nAccélération     : %f\nEfficacité       : %f" % (tSeq, tPar, accel, efficacite))

#
# Programme principal
#
if  __name__  == "__main__":
    main()
