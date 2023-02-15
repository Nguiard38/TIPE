#include "../IntersectionController/IntersectionController.h"

void ecrireTrajFichier(pile* p, int indice_voiture, char* nomFichier)
{
    printf("CoucoU\n");
    char* prePath = "ResultatSimulation/";
    char* path = concat(prePath, nomFichier);
    printf("path : %s\n", path);

    FILE* f = fopen(path, "a");
    free(path);
    fprintf(f, "Voiture %d\n", indice_voiture);
    printf("Test 2\n");

    if(p == NULL)
    {
        fclose(f);
        return;
    }
    noeudPile* current = p->start;
    while(current!= NULL)
    {
        fprintf(f,"%d %d %f\n", current->val.x, current->val.y, current->val.z);
        current = current->next;
    }
    fclose(f);
}

int main(int argc, char * argv[])
{
    srand(time(NULL));
    char* pathLecture = argv[1];
    FILE* fL = fopen(pathLecture, "r");
    parametresCroisements globalParametre;

    fscanf(fL, "%d", &(globalParametre.tailleCroisement));
    printf("Taille Croisement : %d\n", globalParametre.tailleCroisement);

    fscanf(fL, "%d", &globalParametre.nbrDivision);
    printf("Nbr division : %d\n", globalParametre.nbrDivision);

    fscanf(fL, "%d", &globalParametre.vitesseMax);
    printf("Vmax : %d\n", globalParametre.vitesseMax);

    fscanf(fL, "%d", &globalParametre.AccMax);
    printf("TAccMax : %d\n", globalParametre.AccMax);

    fscanf(fL, "%f", &globalParametre.intervalleT);
    printf("intervalleT : %f\n", globalParametre.intervalleT);

    fscanf(fL, "%f", &globalParametre.tailleVoiture);
    printf("tailleVoiture : %f\n", globalParametre.tailleVoiture);

    intersection init = malloc(sizeof(bool*)*globalParametre.nbrDivision);
    
    for(int i = 0; i < globalParametre.nbrDivision; i++)
    {
        init[i] = malloc(sizeof(bool)*globalParametre.nbrDivision);
        for(int j = 0; j < globalParametre.nbrDivision; j++)
        {
            fscanf(fL, "%d", &init[i][j]);
        }
    }
    globalParametre.init = init;
    fclose(fL);

    char* prePathEcriture = "ResultatSimulation/";
    char* pathEcriture = concat(prePathEcriture, argv[2]);
    if(pathEcriture == NULL)
    {
        printf("CHIANT!!!\n");
    }
    printf("path : %s\n", pathEcriture);



    FILE* fR = fopen(pathEcriture, "w");
    if(fR == NULL)
    {
        printf("Erreur d'ouverture\n");
    }
    fprintf(fR, "%d\n", globalParametre.tailleCroisement);
    fprintf(fR, "%d\n", globalParametre.nbrDivision);
    fprintf(fR, "%f\n", globalParametre.intervalleT);
    fprintf(fR, "%f\n", globalParametre.tailleVoiture);

    for(int i = 0; i < globalParametre.nbrDivision; i++)
    {
        for(int j = 0; j < globalParametre.nbrDivision; j++)
        {
            fprintf(fR, "%d ", globalParametre.init[i][j]);
        }
        fprintf(fR, "\n");
    }
    fclose(fR);

    
    int tailleTrottoire = 40; 
    
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

    int repDep = -1;
    int arret = -1;
    nbrVoiture = rand() % 10;
    nbrVoiture -= 7;
    if(nbrVoiture < 0)
    {
        nbrVoiture = 0;
    }

    printf("Nopvre de voitures : %d\n", nbrVoiture);
    while(arret != 0)
    {
        for(int i = 0; i < nbrVoiture; i++)
        {
            position debut;
            int ancienDep;
            ancienDep = repDep;
            nbVoitureTotal++;
            repDep = rand() % 4;
            while(repDep == ancienDep)
            {
                repDep = rand() % 4;
            }
            printf("Depart : %d\n", repDep);
            switch (repDep)
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
            int repFin;
            repFin = rand() % 4;
            while(repFin == repDep)
            {
                repFin = rand() % 4;
            }
            printf("Fin : %d\n", repFin);
            switch (repFin)
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
            vitesse = 0;

            if(vitesse > globalParametre.vitesseMax)
            {
                vitesse = globalParametre.vitesseMax;
            }


            sommet debutT = {debut.x, debut.y, t};
            traj trajVoiture = CalculTraj(c, debutT, fin, vitesse, globalParametre);
        
            c = trajVoiture.c;
            ecrireTrajFichier(trajVoiture.p, nbVoitureTotal, argv[2]);
            afficher_pile(trajVoiture.p);
            free_pile(trajVoiture.p);
            
        }
        
        c = miseAJourCroisement(c, globalParametre);
        t = t + globalParametre.intervalleT;
        nbrVoiture = rand() % 10;
        nbrVoiture -= 7;
        if(nbrVoiture < 0)
        {
            nbrVoiture = 0;
        }
        printf("Nopvre de voitures : %d\n", nbrVoiture);
        arret = rand() % 10;
        
    }
    //free_croisement(c, globalParametre);
}