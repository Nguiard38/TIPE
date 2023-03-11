#include "../IntersectionController/IntersectionController.h"

void ecrireTrajFichier(pile* p, int indice_voiture, char* nomFichier, croisement* c, parametresCroisements globalParametre)
{
    croisement* currentCroisement = c;
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
        //Faire apparaitre le croisement dans le fichier
        
        for(int i = 0; i < globalParametre.nbrDivision; i++)
        {
            for(int j = 0; j < globalParametre.nbrDivision; j++)
            {
                fprintf(f, "%d ", currentCroisement->etat[j][i]);
            }
            fprintf(f, "\n");
        }
        currentCroisement = currentCroisement->next;
        
        current = current->next;
        
    }
    fclose(f);
}

bool surTrottoire(int i, int j, int nbrDivision, int tailleTrottoire)
{
    
    bool basGauche = i < tailleTrottoire && j < tailleTrottoire;
    bool hautGauche = i < tailleTrottoire && j >= nbrDivision-tailleTrottoire;
    bool basDroit = i >= nbrDivision-tailleTrottoire && j < tailleTrottoire;
    bool hautDroit = i >= nbrDivision-tailleTrottoire && j >= nbrDivision-tailleTrottoire;
    return basGauche || hautGauche || basDroit || hautDroit;
}

bool placeLibreACote(intersection etat, int i, int j, parametresCroisements globalParametre, int secu)
{
    for(int x = -secu; x <= secu; x++)
    {
        for(int y = -secu; y <= secu; y++)
        {
            if(x + i >= 0 && x + i < globalParametre.nbrDivision && y + j >= 0 && y + j < globalParametre.nbrDivision && !etat[x+i][y+j])
            {
                return true;
            }
        }
    }
    return false;
}

position EntreUtilisateurDebut(int nbVoitureTotal, parametresCroisements globalParametre, croisement* c)
{
    int rep;
    int tailleTrottoire = 40; 

    int milieu1 = (globalParametre.nbrDivision/2 + tailleTrottoire)/2;
    int milieu2 = (globalParametre.nbrDivision/2 + (globalParametre.nbrDivision - tailleTrottoire ))/2;


    position debut;
    position debutHaut = {milieu1, 0};
    position debutGauche = {0, milieu2};
    position debutBas = {milieu2, globalParametre.nbrDivision-1};
    position debutDroite = {globalParametre.nbrDivision-1, milieu1};


    float tailleCase = ((float)globalParametre.tailleCroisement/(float)globalParametre.nbrDivision);
    int nbrCase = (globalParametre.tailleVoiture - tailleCase)/(2*tailleCase);

    printf("Place d'une voiture case : %d\n", nbrCase);

    printf("Par où la voiture %d arrive ?  (0 = bas, 1 = gauche, 2 = haut, 3 = droite)", nbVoitureTotal);
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

    if(c == NULL)
    {
        while(!placeLibreACote(globalParametre.init, debut.x, debut.y, globalParametre, nbrCase +1))
        {
            printf("Tu ne peux pas commencer par la il y a deja du monde\n");
            printf("Par où la voiture %d arrive ?  (0 = bas, 1 = gauche, 2 = haut, 3 = droite)", nbVoitureTotal);
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
        }
    }
    else
    {
        while(!placeLibreACote(c->etat, debut.x, debut.y, globalParametre, 1))
        {
            printf("Tu ne peux pas commencer par la il y a deja du monde\n");
            printf("Par où la voiture %d arrive ?  (0 = bas, 1 = gauche, 2 = haut, 3 = droite)", nbVoitureTotal);
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
        }
    }
    return debut;
   
}

int main(int argc, char * argv[])
{
    char* pathLecture = argv[1];
    FILE* fL = fopen(pathLecture, "r");
    parametresCroisements globalParametre;

    fscanf(fL, "%d", &globalParametre.tailleCroisement);
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
            int cur;
            fscanf(fL, "%d", &cur);
            init[i][j] = cur;
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
    free(pathEcriture);
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

            debut = EntreUtilisateurDebut(nbVoitureTotal, globalParametre, c);

            position fin;
            printf("Par où la voiture %d repart ?  (0 = bas, 1 = gauche, 2 = haut, 3 = droite)", nbVoitureTotal);
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
            printf("Debut : ");
            afficher_sommet(debutT);
            printf("\nFin : ");
            sommet finAff = {fin.x, fin.y, 0};
            afficher_sommet(finAff);
            printf("\n");
            traj trajVoiture = CalculTraj(c, debutT, fin, vitesse, globalParametre);
            c = trajVoiture.c;

            ecrireTrajFichier(trajVoiture.p, nbVoitureTotal, argv[2], c, globalParametre);
            afficher_pile(trajVoiture.p);
            free_pile(trajVoiture.p);
            
        }
        for(int i = 0; i < 3; i++)
        {
            c = miseAJourCroisement(c, globalParametre);
            t = t + globalParametre.intervalleT;
        }
       
        printf("Combien de voiture voulez vous ajouter ? (t : %f)", t);
        scanf("%d", &nbrVoiture);
    }
    free_croisement(c, globalParametre);
    for(int i = 0; i < globalParametre.nbrDivision; i++)
    {
        
        free(globalParametre.init[i]);
        
    }
    free(globalParametre.init);
    
}