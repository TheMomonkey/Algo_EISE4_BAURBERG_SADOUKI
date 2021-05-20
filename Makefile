CFLAGS=-Wall
INCFLAGS=-lm
EXE=algo1 
EXE1=enumeration

.PHONY: all clean

all: algo1 enumeration

algo1: main.o algo_danger1.o instanciation.o
	@gcc $(CFLAGS) main.o algo_danger1.o instanciation.o -o $@ $(INCFLAGS)
	@echo "Fichier executable cree, nom : $@"

main.o: main.c
	@echo "Compilation de $@"
	@gcc $(CFLAGS) -o $@ -c $< $(INCFLAGS)

algo_danger1.o: algo_danger1.c algo_danger1.h
	@echo "Compilation de $@"
	@gcc $(CFLAGS) -o $@ -c $<
	
enumeration: instanciation.o enumeration.o
	@gcc $(CFLAGS) enumeration.o instanciation.o -o $@ $(INCFLAGS)
	@echo "Fichier executable cree, nom: $@"
	
instanciation.o: instanciation.c instanciation.h
	@echo "Compilation de $@"
	@gcc $(CFLAGS) -o $@ -c $<
	
enumeration.o: enumeration.c
	@echo "Compilation de $@"
	@gcc $(CFLAGS) -o $@ -c $<

clean:
	@echo "Suppression des fichiers objets et de l'executable"
	@rm -f *.o $(EXE) $(EXE1)

