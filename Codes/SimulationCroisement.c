#include "/home/nathan/Prepa/TIPE/Codes/IntersectionController/IntersectionController.h"

int main()
{
    float t = 0.0f;
    int nbrVoiture = 0;
    croisement* c = NULL;

    position debutBas = {60, 0};
    position debutGauche = {0, 40};
    position debutHaut = {40, tailleCroisement-1};
    position debutDroite = {tailleCroisement-1, 60};

    position finBas = {40, 0};
    position finGauche = {0, 60};
    position finHaut = {60, tailleCroisement-1};
    position finDroite = {tailleCroisement-1, 40};

    printf("Combien de voiture voulez vous ajouter ? ");
    scanf("%d", &nbrVoiture);
    while(nbrVoiture != -1)
    {
        for(int i = 0; i < nbrVoiture; i++)
        {
            position debut;
            int rep;
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
                printf("Fin bas");
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

            afficher_pile(trajVoiture.p);
            free_pile(trajVoiture.p);
            
        }

        c = miseAJourCroisement(c);
        t = t + intervalleT;
        printf("Combien de voiture voulez vous ajouter ?");
        scanf("%d", &nbrVoiture);
    }
    free_croisement(c);
}