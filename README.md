# ☢ Problème de stockage de produits toxiques ☢
Mini-projet développé dans le cadre du module d'Algorithmique Avancée (encadré par MM.MARCHETTI Olivier et OUZIA Hacene) de la spécialité Electronique et Informatique parcours Systèmes Embarqués année 4 à Polytech Sorbonne.

# Sujet
## Contexte
L'objectif est, à partir d'un **graphe** (non orienté, connexe ou non) dont les **sommets** sont des **fûts** remplis de produits toxiques (en nombre égal à un carré) et les **arêtes** entre les sommets décrivent le **risque de réaction (binaire)**, de proposer une **répartition** des fûts sur une **grille carrée bidimensionnelle**. Celle-ci doit minimiser le nombre de fûts qui sont voisins sur la grille ET présentent une arête dans le graphe initial (possible réaction chimique ☠️).

## Exemple
<p align="center">
  <img src="https://user-images.githubusercontent.com/36695513/119200054-78cfbc80-ba8c-11eb-8cbe-a4ab0547fdeb.png" />
</p>

Voici un exemple présenté dans le sujet où 16 fûts sont entreposés. Il vient donc que cet entreposage présente une dangerosité de **11** puisqu'il y a à droite 11 voisinages qui sont décrits par une arête dans le graphe de gauche.

## Implémentation
### Solution 1 - force brute (énumération)
Cette solution énumère toutes les configurations de matrices possibles et fait ressortir la meilleure (solution exacte). L'algorithme reprend celui étudié en TD avec M.OUZIA.

**Code :** `enumeration.c + instanciation.c`

**Exécution :**
``` bash
$ make enumeration
$ ./enumeration <nb_sommets> <degre_maximal>
```

### Solution 2 - méthode heuristique (intuition)
Une amélioration consiste à exploiter les propriétés d'un carré puisque le nombre de voisins directs diffère selon l'emplacement sur la matrice (coins et bords notamment).

**Code :** `main.c + algo_danger1.c + instanciation.c`

**Exécution :**
``` bash
$ make algo1
$ ./algo1 <nb_sommets> <degre_maximal>
```
&rArr; retourne **l'affichage graphique** de la matrice ainsi que la **dangerosité** de l'entreposage ainsi généré dans un fichier `matrice.txt` (créé automatiquement dans le répertoire courant).

## Bugs
- La solution 1 n'est pas fonctionnelle pour des graphes supérieurs à 4 sommets. A voir si seulement très long (pas de problème) ou infini.

## Améliorations possibles
- La génération d'une instance de graphe est longue. Il faudrait ajouter une fonctionnalité permettant à l'utilisateur d'entrer directement le graphe de description.
