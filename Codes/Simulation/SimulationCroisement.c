#include "/home/nathan/Prepa/TIPE/Codes/IntersectionController/IntersectionController.h"

int main(int argc, char * argv[])
{
    float t = 0.0f;
    int nbrVoiture = 0;
    int nbVoitureTotal = 0;
    croisement* c = NULL;

    int milieu1 = (tailleCroisement/2 + tailleTrottoire)/2;
    int milieu2 = (tailleCroisement/2 + (tailleCroisement - tailleTrottoire ))/2;

    position debutHaut = {milieu1, 0};
    position debutGauche = {0, milieu2};
    position debutBas = {milieu2, tailleCroisement-1};
    position debutDroite = {tailleCroisement-1, milieu1};

    position finHaut = {milieu2, 0};
    position finGauche = {0, milieu1};
    position finBas = {milieu1, tailleCroisement-1};
    position finDroite = {tailleCroisement-1, milieu2};

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

            if(vitesse > vitesseMax)
            {
                vitesse = vitesseMax;
            }


            sommet debutT = {debut.x, debut.y, t};
            traj trajVoiture = CalculTraj(c, debutT, fin, vitesse);
            c = trajVoiture.c;

            afficher_pile(trajVoiture.p, nbVoitureTotal,argv[1]);
            free_pile(trajVoiture.p);
            
        }

        c = miseAJourCroisement(c);
        t = t + intervalleT;
        printf("Combien de voiture voulez vous ajouter ? (t : %f)", t);
        scanf("%d", &nbrVoiture);
    }
    free_croisement(c);
}