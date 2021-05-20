CFLAGS=-Wall
EXE=algo1 
EXE1=enumeration

.PHONY: all clean

all: algo1 enumeration

algo1: main.o algo_danger1.o
	@gcc $(CFLAGS) main.o algo_danger1.o -o $@
	@echo "Fichier executable cree, nom : $@"

main.o: main.c
	@echo "Compilation de $@"
	@gcc $(CFLAGS) -o $@ -c $<

algo_danger1.o: algo_danger1.c algo_danger1.h
	@echo "Compilation de $@"
	@gcc $(CFLAGS) -o $@ -c $<
	
enumeration: instanciation.o enumeration.o
	@gcc $(CFLAGS) -lm enumeration.o instanciation.o -o $@
	@echo "Fichier executable cree, nom: $@"
	
instanciation.o: instanciation.c instanciation.h
	@echo "Compilation de $@"
	@gcc $(CFLAGS) -o $@ -c $<
	
enumeration.o: enumeration.c
	@echo "Compilation de $@"
	@gcc $(CFLAGS) -o $@ -c $<

clean:
	@echo "Suppression des fichiers objets et de l'executable"
	@rm -f *.o $(EXE)

