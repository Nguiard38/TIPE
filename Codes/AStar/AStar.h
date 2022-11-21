#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

typedef struct sommet {
    int x;
    int y;
    float z;
} sommet;

void afficher_sommet(sommet s);

typedef struct noeud {
    sommet s;
    float cout;
    float heuristic;
    float v;
    struct noeud* prec;
} noeud;

//--------Comparaison Sommets-----------
bool memeSommet(sommet A, sommet B);
float distance3D(sommet A, sommet B);

//----------GRAPHE---------
typedef struct tupleG {
    sommet p;
    sommet* arretes;
    int nombreArretes;
} tupleG;

typedef struct graphe {
    int nombreSommets;
    tupleG* liaisons;
} graphe;

//----------PILE-------------
typedef struct noeudPile {
    sommet val;
    struct noeudPile* next;
} noeudPile;

typedef struct pile {
    noeudPile* start;
} pile;

pile* creer_pile_vide();
bool pile_est_vide(pile* p);
void ajouter_pile(pile* p, sommet A);
sommet depiler(pile* p);
void free_pile(pile* p);
void afficher_pile(pile* p);

//-----------FILE--------------
typedef struct noeudFile {
    noeud* val;
    struct noeudFile* next;
} noeudFile;

typedef struct file {
    noeudFile* first;
    noeudFile* last;
} file;

file* creer_file_vide();
bool file_est_vide(file* f);
void ajouter_file(file* f, noeud* n);
void ajouter_prio(file* f, noeud* n);
bool appartient(file* f, noeud* n);
void mettreAJour(file* f, noeud* n);
noeud* defiler(file* f);
void free_file(file* f);

//-----------Algo------------------
typedef struct path {
    int taille;
    pile* p;
} path;

path reconstituerChemin(sommet debut, noeud* fin);
file* trouverVoisinsContraintes(graphe* g, noeud* u);
path Astar(graphe* g, sommet debut, sommet fin, float v);