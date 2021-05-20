CFLAGS=-Wall
INCFLAGS += -lm
EXE=algo1 
EXE1=enumeration

.PHONY: all clean

all: algo1 enumeration

algo1: main.c algo_danger1.c instanciation.c
	@gcc $(CFLAGS) main.c algo_danger1.c instanciation.c -o $@ $(INCFLAGS)
	@echo "Fichier executable cree, nom : $@"

main.o: main.c
	@echo "Compilation de $@"
	@gcc $(CFLAGS) $(INCFLAGS) -o $@ -c $<

algo_danger1.o: algo_danger1.c algo_danger1.h
	@echo "Compilation de $@"
	@gcc $(CFLAGS) -o $@ -c $<
	
enumeration: instanciation.c enumeration.c
	@gcc $(CFLAGS) enumeration.c instanciation.c -o $@ $(INCFLAGS)
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

