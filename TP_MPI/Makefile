CC = mpicc                   # Compilateur utilisé
CFLAGS = -Wall               # Options de compilation
LIBS = -lm                   # Bibliothèque à ajouter (maths)
SOURCES = $(wildcard *.c)    # Liste des fichiers sources (!! suppose 1 seul fichier par programme !!)
PROGS = $(SOURCES:%.c=%)     # Liste des programmes à compiler

MAC = no                     # Mettre yes si vous utilisez MAC OS

ifeq ($(MAC), yes)
	CFLAGS += -openmp
else
	CFLAGS += -fopenmp
endif

all: $(PROGS)                # Cible générale de compilation

%: %.c                       # Règle de compilation pour chaque fichier C
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:                       # Règle de nettoyage du dossier courant -> SUPPRESsION des fichiers intermédiaires de compilation
	rm -f *~ *.o

cleanall: clean              # Règle de nettoyage du dossier courant avec SUPPRESSION des exécutables
	rm -f $(PROGS)
