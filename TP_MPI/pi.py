#!/bin/python3
# -*- coding: utf-8 -*-

# Programme qui calcule une approximation de PI avec MPI
# Execution : invocation directe dans un terminal ou via python3

# Bibliothèques utilisées
import sys
import math

# Import de MPI
from mpi4py import MPI

#
# Équation du cercle unitaire
#
def cercle(x):
    return math.sqrt(1.0 - x * x)

#
# Intégration partielle du quart de disque positif à partir de la position debut avec nLoc trapèzes de largeur dx
#
def integration(nLoc, debut, dx):
    res = (cercle(debut) + cercle(debut + nLoc * dx)) / 2.0 # Contributions des points aux extrémités

    x = debut                # Position x de départ de l'intégration partielle
    for i in range(1, nLoc): # Contributions des points internes
        x   += dx
        res += cercle(x);

    return 4.0 * dx * res;

#
# Fonction principale
#
def main():

    # Valeur par défaut du nombre de trapèzes
    n       = 1000000 # Nombre de trapèzes
    
    # Paramètres de la ligne de commande
    argc = len(sys.argv)
    for i in range(argc):
        if sys.argv[i] == "-n":
            i = i + 1
            n = int(sys.argv[i])
    
    # Variables internes du programme
    pi      = 0.0     # Valeur finale de PI
    pi_part = 0.0     # Valeur partielle de PI calculée par le processus courant
    tdeb    = 0.0     # Temps de début du calcul
    tfin    = 0.0     # Temps de fin du calcul
    debut   = 0.0     # Position x du 1er trapèze de l'intégration partielle
    dx      = 1.0 / n # Largeur des trapèzes
    nLoc    = n       # Nombre de trapèzes locaux par processus (mis à jour ci-dessous)

    # Initialisation MPI
    comm = MPI.COMM_WORLD  # Groupe de communication
    idP  = comm.Get_rank() # Identifiant du processus
    nbP  = comm.Get_size() # Nombre de processus dans le groupe

    # Temps de début du calcul
    tdeb = MPI.Wtime()

    # Calcul des paramètres locaux : nb trapèzes et indices
    nLoc   = n   # !! À MODIFIER !!
    reste  = 0   # !! À MODIFIER !!
    debut  = 0.0 # !! À MODIFIER !!

    # Integration partielle du quart de disque supérieur-droit à partir de la position debut avec nLoc trapèzes de largeur dx
    pi_part = integration(nLoc, debut, dx)

    # Affichage des résultats partiels
    print("Le processus %d intègre %d trapèzes de largeur %f depuis la position %f : %.15f" % (idP, nLoc, dx, debut, pi_part))

    # Fusion globale des résultats partiels
    pi = pi_part # !! À MODIFIER !!

    # Temps de fin du calcul
    tfin = MPI.Wtime()
    
    # Affichage du résulat global par le processus 0
    if (idP == 0):
        print("Approximation de PI             : %1.15f" % (pi))
        print("Différence avec valeur standard : %.3e" % (math.pi - pi))
        print("Calcul en                       : %fs" % (tfin - tdeb))

#
# Programme principal
#
if  __name__  == "__main__":
    main()
