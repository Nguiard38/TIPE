#include "IntersectionController.h"



void free_croisement(croisement* c, parametresCroisements globalParametre)
{
    croisement* current = c;
    while(current != NULL)
    {
        croisement* aLib = current;
        current = current->next;
        for(int i = 0; i < globalParametre.nbrDivision; i++)
        {
            free(aLib->etat[i]);
        }
        free(aLib->etat);
        free(aLib);

    }
}

croisement* miseAJourCroisement(croisement* c, parametresCroisements globalParametre)
{
    if(c != NULL)
    {
        c = c->next;
       
        for(int i = 0; i < globalParametre.nbrDivision; i++)
        {
            //free(c->prec->etat[i]);
        }
        free(c->prec->etat);
        free(c->prec);

        
        return c;
    }
    else
    {
        return NULL;
    }
}

traj reconstituerTraj(sommet debut, noeud* fin, croisement* c, parametresCroisements globalParametre)
{
    printf("Reconstituer Chemin\n");
    noeud* actual = fin;
    croisement* current = c;
    croisement* currentPrec = NULL;
    float t;

    if(current == NULL)
    {
        t = debut.z;
    }
    else
    {
        while(current != NULL && current->t0 < fin->s.z)
        {
            currentPrec = current;
            current = current->next;
        }
        t = currentPrec->t0;
        
        
    }
    

    while(t < fin->s.z)
    {
        current = malloc(sizeof(croisement));
        current->etat = malloc(sizeof(bool*) * globalParametre.nbrDivision);
        for(int i = 0; i < globalParametre.nbrDivision; i++)
        {
            current->etat[i] = malloc(sizeof(bool) * globalParametre.nbrDivision);
            for(int j = 0; j < globalParametre.nbrDivision; j++)
            {
                current->etat[i][j] = globalParametre.init[i][j];
            }
        }
        current->t0 = t;
        current->next = NULL;
        current->prec = currentPrec;

        if(currentPrec != NULL)
        {
            currentPrec->next = current;
        }
        
        
        currentPrec = current;
        current = NULL;
        t = t + globalParametre.intervalleT;
    }
    printf("t : %f\n", t);
    printf("tfin : %f\n", actual->s.z);
    current = currentPrec;
    int taille = 1;
    pile* p = creer_pile_vide();
    while(!memeSommet(actual->s, debut))
    {
        taille++;
        //Mettre a jour la hitbox de la voiture
        float tailleCase = ((float)globalParametre.tailleCroisement/(float)globalParametre.nbrDivision);
        int nbrCase = (globalParametre.tailleVoiture - tailleCase)/(2*tailleCase) + 1;
        if(nbrCase < 0)
        {
            nbrCase = 0;
        }
        for(int i = actual->s.x-nbrCase; i <= nbrCase+actual->s.x; i++)
        {
            for(int j = actual->s.y-nbrCase; j <= actual->s.y+nbrCase; j++)
            {
                if(i >= 0 && i < globalParametre.nbrDivision)
                {
                    current->etat[i][j] = true;
                }
            }
        }

        if(current->prec != NULL)
        {
            current = current->prec;
        }
        
        ajouter_pile(p, actual->s);
        actual = (actual->prec);
    }
    current->etat[debut.x][debut.y] = true;
    ajouter_pile(p, debut);
    traj res = {taille, p, current};
    printf("Res reconstitue traj\n");
    return res;
}

float distance2D(position A, position B)
{
    float ux = B.x - A.x;
    float uy = B.y - A.y;
    float distance = sqrt(pow(ux, 2) + pow(uy,2));
    return distance;
}

float vitesse(sommet A, sommet B, parametresCroisements globalParametre)
{
    float tailleCase = ((float)globalParametre.tailleCroisement/(float)globalParametre.nbrDivision);
    float ux = (B.x - A.x)*tailleCase;
    float uy = (B.y - A.y)*tailleCase;
    float deltaT = B.z - A.z;
    float distance = sqrt(pow(ux, 2) + pow(uy,2));
    return distance/deltaT;
}

float acc(noeud* A, sommet B, parametresCroisements globalParametre)
{
    return A->v - vitesse(A->s, B, globalParametre);
}

bool aCoteOccupe(intersection etat, int i, int j,parametresCroisements globalParametre)
{
    int secu = 2;
    for(int x = -secu; x <= secu; x++)
    {
        for(int y = -secu; y <= secu; y++)
        {
            if(x + i >= 0 && x + i < globalParametre.nbrDivision && y + j >= 0 && y + j < globalParametre.nbrDivision && etat[x+i][y+j])
            {
                return true;
            }
        }
    }
    return false;
}

file* prochainPoints(croisement* c, noeud* u, parametresCroisements globalParametre)
{
    file* voisins = creer_file_vide();
    int rayonMax = globalParametre.vitesseMax/globalParametre.intervalleT;
    for(int i = -rayonMax; i < rayonMax; i ++)
    {
        for(int j = -rayonMax; j < rayonMax; j++)
        {
            if(u->s.x + i >= 0 && u->s.x + i < globalParametre.nbrDivision && u->s.y + j >= 0 && u->s.y + j < globalParametre.nbrDivision)
            {
                noeud* new = malloc(sizeof(noeud));
                new->s.x = u->s.x + i;
                new->s.y = u->s.y + j;
                new->s.z = u->s.z + globalParametre.intervalleT;
                new->cout = 0;
                new->heuristic = 0;
                new->v = vitesse(u->s, new->s, globalParametre);
                new->prec = NULL;
                if(new->v <= globalParametre.vitesseMax && sqrt(pow(acc(u, new->s,globalParametre),2)) <= globalParametre.AccMax)
                {
                    if(c != NULL)
                    {
                        if(!aCoteOccupe(c->etat, u->s.x + i, u->s.y + j, globalParametre))
                        {
                            ajouter_file(voisins, new);
                        }
                        else 
                        {
                            free(new);
                        }
                    }
                    else
                    {
                        if(!aCoteOccupe(globalParametre.init, u->s.x + i, u->s.y + j, globalParametre))
                        {
                            ajouter_file(voisins, new);
                        }
                        else 
                        {
                            free(new);
                        }
                    }
                }
                else
                {
                    free(new);
                }
            }
        }
    }
    return voisins;

}

traj CalculTrajAvecFin(croisement* c, sommet debut, sommet fin, float v, parametresCroisements globalParametre)
{
    traj pasChemin;
    croisement* current = c;
    file* closed = creer_file_vide();
    file* open = creer_file_vide();
    noeud* nDebut = malloc(sizeof(noeud));
    nDebut->s = debut;
    nDebut->cout = 0;
    nDebut->heuristic = distance3D(debut, fin);
    nDebut->v = v;
    nDebut->prec = NULL;
    ajouter_file(open, nDebut);
    
    while(!file_est_vide(open))
    {
        if(current != NULL)
        {
            current = current->next;
        }

        noeud* u = defiler_prio(open);
        ajouter_file(closed, u);
        if(memeSommet(u->s, fin))
        {
            traj res = reconstituerTraj(debut, u, c, globalParametre);
            //free_file(closed);
            //free_file(open);
            return res;
        }
        if(u->s.z >= fin.z)
        {
            position A;
            A.x = u->s.x;
            A.y = u->s.y;
            position B;
            B.x = fin.x;
            B.y = fin.y;
            pasChemin.taille = (distance2D(A, B)/globalParametre.vitesseMax);
            free_file(closed);
            free_file(open);
            break;
        }


        file* voisins = prochainPoints(current, u, globalParametre);

        while(!file_est_vide(voisins))
        {

            noeud* v = defiler(voisins);
            v->cout = u->cout + distance3D(u->s, v->s)/2;
            v->heuristic = v->cout + distance3D(v->s, fin);
            v->prec = u;
            if(!appartient(closed, v))
            {
                mettreAJour(open,v);
            }
            else
            {
                free(v);
            }  
        }
        free(voisins);
        
    }
    
    pasChemin.p = NULL;
    pasChemin.c = c;
   // printf("Chemin non existant\n");
    return pasChemin;

}

traj CalculTraj(croisement* c, sommet debut, position fin, float v, parametresCroisements globalParametre)
{
    printf("Calcul traj\n");
    float ux = fin.x - debut.x;
    float uy = fin.y - debut.y;
    float distance = sqrt(pow(ux, 2) + pow(uy,2));
    int tempsMin = distance/globalParametre.vitesseMax;
    float tempsEssayer = tempsMin;
    traj res;
    res.taille = 0;
    res.p = NULL;
    while(res.p == NULL)
    {
        sommet finTemps = {fin.x, fin.y, tempsEssayer};
        res = CalculTrajAvecFin(c, debut, finTemps, v, globalParametre);
        tempsEssayer = tempsEssayer + globalParametre.intervalleT;
        if(res.taille == 0)
        {
            tempsEssayer = tempsEssayer + globalParametre.intervalleT;
        }
        else
        {
            tempsEssayer = tempsEssayer + res.taille*globalParametre.intervalleT;
        }
        
    }
    return res;
}


