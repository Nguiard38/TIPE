#include "../IntersectionController/IntersectionController.h"

bool surTrottoire(int i, int j, int nbrDivision, int tailleTrottoire)
{
    
    bool basGauche = i < tailleTrottoire && j < tailleTrottoire;
    bool hautGauche = i < tailleTrottoire && j >= nbrDivision-tailleTrottoire;
    bool basDroit = i >= nbrDivision-tailleTrottoire && j < tailleTrottoire;
    bool hautDroit = i >= nbrDivision-tailleTrottoire && j >= nbrDivision-tailleTrottoire;
    return basGauche || hautGauche || basDroit || hautDroit;
}

int main(int argc, char * argv[])
{
    int tailleTrottoire =40; 
    parametresCroisements globalParametre;
    globalParametre.tailleCroisement = 100;
    globalParametre.nbrDivision = 100;
    globalParametre.vitesseMax = 14;
    globalParametre.AccMax = 5;
    globalParametre.intervalleT = 0.2;
    globalParametre.tailleVoiture = 2;

    intersection init = malloc(sizeof(bool*)*globalParametre.nbrDivision);
    
    for(int i = 0; i < globalParametre.nbrDivision; i++)
    {
        init[i] = malloc(sizeof(bool)*globalParametre.nbrDivision);
        for(int j = 0; j < globalParametre.nbrDivision; j++)
        {
            init[i][j] = surTrottoire(i, j, globalParametre.nbrDivision, tailleTrottoire);
        }
    }

    globalParametre.init = init;
    float t = 0.0f;
    int nbrVoiture = 0;
    int nbVoitureTotal = 0;
    croisement* c = NULL;

    int milieu1 = (globalParametre.nbrDivision/2 + tailleTrottoire)/2;
    int milieu2 = (globalParametre.nbrDivision/2 + (globalParametre.nbrDivision - tailleTrottoire ))/2;

    position debutHaut = {milieu1, 0};
    position debutGauche = {0, milieu2};
    position debutBas = {milieu2, globalParametre.nbrDivision-1};
    position debutDroite = {globalParametre.nbrDivision-1, milieu1};

    position finHaut = {milieu2, 0};
    position finGauche = {0, milieu1};
    position finBas = {milieu1, globalParametre.nbrDivision-1};
    position finDroite = {globalParametre.nbrDivision-1, milieu2};

    printf("Combien de voiture voulez vous ajouter ? (t : %f)", t);
    scanf("%d", &nbrVoiture);
    while(nbrVoiture != -1)
    {
        for(int i = 0; i < nbrVoiture; i++)
        {
            position debut;
            int rep;
            nbVoitureTotal++;
            printf("Par où la voiture %d arrive ?  (0 = bas, 1 = gauche, 2 = haut, 3 = droite)", i);
            scanf("%d", &rep);
            switch (rep)
            {
            case 0:
                debut = debutBas;
                break;
            case 1:
                debut = debutGauche;
                break;
            case 2:
                debut = debutHaut;
                break;
            case 3:
                debut = debutDroite;
                break;
            default:
                debut = debutBas;
                break;
            }

            position fin;
            printf("Par où la voiture %d repart ?  (0 = bas, 1 = gauche, 2 = haut, 3 = droite)", i);
            scanf("%d", &rep);
            switch (rep)
            {
            case 0:
                fin = finBas;
                break;
            case 1:
                fin = finGauche;
                break;
            case 2:
                fin = finHaut;
                break;
            case 3:
                fin = finDroite;
                break;
            default:
                fin = finBas;
                break;
            }

            float vitesse;
            printf("A quelle vitesse votre voiture arrive dans l'intersection ?");
            scanf("%f", &vitesse);

            if(vitesse > globalParametre.vitesseMax)
            {
                vitesse = globalParametre.vitesseMax;
            }


            sommet debutT = {debut.x, debut.y, t};
            printf("Taille croisement : %d\n", globalParametre.nbrDivision);
            traj trajVoiture = CalculTraj(c, debutT, fin, vitesse, globalParametre);
            c = trajVoiture.c;

            afficher_pile(trajVoiture.p, nbVoitureTotal,argv[1]);
            printf("Wow\n");
            free_pile(trajVoiture.p);
            
        }
        
        c = miseAJourCroisement(c, globalParametre);
        t = t + globalParametre.intervalleT;
        printf("Combien de voiture voulez vous ajouter ? (t : %f)", t);
        scanf("%d", &nbrVoiture);
    }
    free_croisement(c, globalParametre);
}