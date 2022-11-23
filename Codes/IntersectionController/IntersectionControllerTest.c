#include "IntersectionController.h"



int main()
{
    sommet debut = {60, 0, 0};
    sommet debut2 = {40, tailleCroisement-1, 0};
    position finBas = {40, 0};
    position finGauche = {0, 60};
    position finHaut = {60, tailleCroisement-1};
    position finDroite = {tailleCroisement-1, 40};
    croisement* c = NULL;
    traj Voiture1 = CalculTraj(c, debut, finDroite, 0);
    c = Voiture1.c;
    if(Voiture1.p != NULL)
    {
        afficher_pile(Voiture1.p);
        free_pile(Voiture1.p);
    }

    
    traj Voiture2 = CalculTraj(c, debut2, finBas, 0);
    c = Voiture2.c;
 
    if(Voiture2.p != NULL)
    {
        afficher_pile(Voiture2.p);
        free_pile(Voiture2.p);
    }
    if(c != NULL)
    {
        free_croisement(c);
    }
    
}