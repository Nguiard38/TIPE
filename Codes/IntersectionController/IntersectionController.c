#include "IntersectionController.h"


void free_croisement(croisement* c)
{
    croisement* current = c;
    while(current != NULL)
    {
        croisement* aLib = current;
        current = current->next;
        free(aLib);
    }
}

croisement* miseAJourCroisement(croisement* c)
{
    if(c != NULL)
    {
        c = c->next;
        free(c->prec);
        return c;
    }
    else
    {
        return NULL;
    }
}

traj reconstituerTraj(sommet debut, noeud* fin, croisement* c)
{
    printf("Reconstituer Chemin\n");
    noeud* actual = fin;
    croisement* current = c;
    if(current == NULL)
    {
        current = malloc(sizeof(croisement));
        current->t0 = debut.z;
        current->prec = NULL;
        current->next = NULL;

        //Init trottoire
        for(int i = 0; i < tailleCroisement; i++)
        {
            for(int j = 0; j < tailleCroisement; j++)
            {
                current->etat[i][j] = false;
            }
        }
    }
    while(current->next != NULL && current->t0 < fin->s.z)
    {
        current = current->next;
    }
    while(current->t0 < fin->s.z)
    {
        current->next = malloc(sizeof(croisement));
        current->next->t0 = current->t0 + intervalleT;
        current->next->next = NULL;
        current->next->prec = current;
        for(int i = 0; i < tailleCroisement; i++)
        {
            for(int j = 0; j < tailleCroisement; j++)
            {
                current->etat[i][j] = false;
            }
        }
        current = current->next;
    }
    int taille = 1;
    pile* p = creer_pile_vide();
    while(!memeSommet(actual->s, debut))
    {
        taille++;
        current->etat[actual->s.x][actual->s.y] = true;
        if(!(actual->s.x > 0))
        {
            if((actual->s.y > 0))
            {
                current->etat[actual->s.x][actual->s.y-1] = true;
            }
        }
        else
        {
            if(actual->s.y > 0)
            {
                current->etat[actual->s.x-1][actual->s.y-1] = true;
            }
            current->etat[actual->s.x-1][actual->s.y] = true;
            
        }
       
        

        
        current = current->prec;
        ajouter_pile(p, actual->s);
        actual = (actual->prec);
    }
    current->etat[debut.x][debut.y] = true;
    ajouter_pile(p, debut);
    traj res = {taille, p, current};
    return res;
}

float distance2D(position A, position B)
{
    float ux = B.x - A.x;
    float uy = B.y - A.y;
    float distance = sqrt(pow(ux, 2) + pow(uy,2));
    return distance;
}

float vitesse(sommet A, sommet B)
{
    float ux = B.x - A.x;
    float uy = B.y - A.y;
    float deltaT = B.z - A.z;
    float distance = sqrt(pow(ux, 2) + pow(uy,2));
    return distance/deltaT;
}

float acc(noeud* A, sommet B)
{
    return A->v - vitesse(A->s, B);
}

bool surTrottoire(int i, int j)
{
    bool basGauche = i < tailleTrottoire && j < tailleTrottoire;
    bool hautGauche = i < tailleTrottoire && j >= tailleCroisement-tailleTrottoire;
    bool basDroit = i >= tailleCroisement-tailleTrottoire && j < tailleTrottoire;
    bool hautDroit = i >= tailleCroisement-tailleTrottoire && j >= tailleCroisement-tailleTrottoire;
    return basGauche || hautGauche || basDroit || hautDroit;
}

file* prochainPoints(croisement* c, noeud* u)
{
    file* voisins = creer_file_vide();
    int rayonMax = vitesseMax/intervalleT;
    for(int i = -rayonMax; i < rayonMax; i ++)
    {
        for(int j = -rayonMax; j < rayonMax; j++)
        {
            if(u->s.x + i >= 0 && u->s.x + i < tailleCroisement && u->s.y + j >= 0 && u->s.y + j < tailleCroisement)
            {
                noeud* new = malloc(sizeof(noeud));
                new->s.x = u->s.x + i;
                new->s.y = u->s.y + j;
                new->s.z = u->s.z + intervalleT;
                new->cout = 0;
                new->heuristic = 0;
                new->v = vitesse(u->s, new->s);
                new->prec = NULL;
                if(new->v <= vitesseMax && sqrt(pow(acc(u, new->s),2)) <= AccMax)
                {
                    if(c == NULL  || !c->etat[u->s.x + i][u->s.y + j])
                    {
                        if(!surTrottoire(u->s.x + i, u->s.y + j))
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
                        free(new);
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

traj CalculTrajAvecFin(croisement* c, sommet debut, sommet fin, float v)
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
            printf("Trouver chemin\n");
            traj res = reconstituerTraj(debut, u, c);
            free_file(closed);
            free_file(open);
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
            pasChemin.taille = distance2D(A, B)/vitesseMax;
            free_file(closed);
            free_file(open);
            break;
        }
        file* voisins = prochainPoints(current, u);
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
    printf("Chemin non existant\n");
    return pasChemin;

}

traj CalculTraj(croisement* c, sommet debut, position fin, float v)
{
    float ux = fin.x - debut.x;
    float uy = fin.y - debut.y;
    float distance = sqrt(pow(ux, 2) + pow(uy,2));
    int tempsMin = distance/vitesseMax;
    float tempsEssayer = tempsMin;
    traj res;
    res.taille = 0;
    res.p = NULL;
    while(res.p == NULL)
    {
        sommet finTemps = {fin.x, fin.y, tempsEssayer};
        res = CalculTrajAvecFin(c, debut, finTemps, v);
        if(res.taille == 0)
        {
            tempsEssayer = tempsEssayer + intervalleT;
        }
        else
        {
            tempsEssayer = tempsEssayer + res.taille;
        }
        
    }
    return res;
}


