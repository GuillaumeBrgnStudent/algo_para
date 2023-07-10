CC = gcc                     # Compilateur utilisé
CFLAGS = -Wall -fopenmp      # Options de compilation
DFLAGS = -ggdb -g -pg        # Options de debuggage et profilage
LIBS = -lm                   # Bibliothèque à ajouter (maths)
SOURCES = $(wildcard *.c)    # Liste des fichiers sources (!! suppose 1 seul fichier par programme !!)
PROGS = $(SOURCES:%.c=%)     # Liste des programmes à compiler

MAC = no                     # Mettre yes si vous utilisez MAC OS
							 # Il faut également installer llvm :
							 # brew install llvm
ifeq ($(MAC), yes)
	CC = /usr/local/opt/llvm/bin/clang
	CFLAGS += -I/usr/local/opt/llvm/include
	LIBS += -L/usr/local/opt/llvm/lib
endif

all: $(PROGS)                # Cible générale de compilation

%: %.c                       # Règle de compilation pour chaque fichier C
	$(CC) $(CFLAGS) $(DFLAGS) $< -o $@ $(LIBS)

clean:                       # Règle de nettoyage du dossier courant -> SUPPRESsION des fichiers intermédiaires de compilation
	rm -f *~ *.o

cleanall: clean              # Règle de nettoyage du dossier courant avec SUPPRESSION des exécutables
	rm -f $(PROGS)
