#include "/home/nathan/Prepa/TIPE/Codes/AStar/AStar.h"

#define tailleCroisement 100
#define tailleTrottoire 40
#define vitesseMax 14
#define AccMax 5
#define intervalleT 0.5
#define tailleVoiture 2

typedef bool intersection[tailleCroisement][tailleCroisement];

typedef struct croisement {
    intersection etat;
    float t0;
    struct croisement* prec;
    struct croisement* next;
} croisement;

typedef struct traj {
    int taille;
    pile* p;
    croisement* c;
} traj;

typedef struct position {
    int x;
    int y;
} position;


void free_croisement(croisement* c);
croisement* miseAJourCroisement(croisement* c);
traj reconstituerTraj(sommet debut, noeud* fin, croisement* c);
float distance2D(position A, position B);
float vitesse(sommet A, sommet B);
float acc(noeud* A, sommet B);
bool surTrottoire(int i, int j);
file* prochainPoints(croisement* c, noeud* u);
traj CalculTrajAvecFin(croisement* c, sommet debut, sommet fin, float v);
traj CalculTraj(croisement* c, sommet debut, position fin, float v);