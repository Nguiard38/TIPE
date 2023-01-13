#include "../AStar/AStar.h"

typedef bool** intersection;

typedef struct parametresCroisements 
{
    int tailleCroisement;
    int nbrDivision;
    intersection init;
    int vitesseMax;
    int AccMax;
    float intervalleT;
    float tailleVoiture;

} parametresCroisements;

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


void free_croisement(croisement* c, parametresCroisements globalParametre);
croisement* miseAJourCroisement(croisement* c, parametresCroisements globalParametre);
traj reconstituerTraj(sommet debut, noeud* fin, croisement* c, parametresCroisements globalParametre);
float distance2D(position A, position B);
float vitesse(sommet A, sommet B, parametresCroisements globalParametre);
float acc(noeud* A, sommet B, parametresCroisements globalParametre);
file* prochainPoints(croisement* c, noeud* u, parametresCroisements globalParametre);
traj CalculTrajAvecFin(croisement* c, sommet debut, sommet fin, float v, parametresCroisements globalParametre);
traj CalculTraj(croisement* c, sommet debut, position fin, float v, parametresCroisements globalParametre);