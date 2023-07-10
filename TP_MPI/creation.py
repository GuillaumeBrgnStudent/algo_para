#!/bin/python3
# -*- coding: utf-8 -*-

# Programme MPI simple
# Execution : invocation directe dans un terminal ou via python3

# Import de MPI
from mpi4py import MPI

# Fonction principale
def main():

    comm = MPI.COMM_WORLD  # Groupe de communication
    idP  = comm.Get_rank() # Identifiant du processus
    nbP  = comm.Get_size() # Nombre de processus

    # Affichage des processus
    print("Processus %d parmi %d" % (idP, nbP))

# Programme principal
if  __name__  == "__main__":
    main()
