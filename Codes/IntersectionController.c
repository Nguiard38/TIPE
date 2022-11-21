#include "AStar/AStar.h"

#define tailleCroisement 100
#define vitesseMax 14
#define AccMax 5
#define intervalleT 0.5

typedef bool intersection[tailleCroisement][tailleCroisement];

typedef struct croisement {
    intersection etat;
    float t0;
    struct croisement* prec;
    struct croisement* next;
} croisement;

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

typedef struct traj {
    int taille;
    pile* p;
    croisement* c;
} traj;

typedef struct position {
    int x;
    int y;
} position;

void miseAJourCroisement(croisement* c)
{
    if(c != NULL)
    {
        c = c->next;
        free(c->prec);
    }
}

traj reconstituerTraj(sommet debut, noeud* fin, croisement* c)
{
    noeud* actual = fin;
    croisement* current = c;
    if(current == NULL)
    {
        current = malloc(sizeof(croisement));
        current->t0 = debut.z;
        current->prec = NULL;
        current->next = NULL;
        for(int i = 0; i < tailleCroisement; i++)
        {
            for(int j = 0; j < tailleCroisement; j++)
            {
                current->etat[i][j] = false;
            }
        }
    }
    while(current->next != NULL)
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
        current = current->prec;
        ajouter_pile(p, actual->s);
        actual = (actual->prec);
    }
    current->etat[debut.x][debut.y] = true;
    ajouter_pile(p, debut);
    traj res = {taille, p, current};
    return res;
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

file* prochainPoints(croisement* c, noeud* u)
{
    file* voisins = creer_file_vide();
    int rayonMax = vitesseMax/intervalleT;
    for(int i = -rayonMax; i < rayonMax; i ++)
    {
        for(int j = -rayonMax; j < rayonMax; j++)
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
                if(c == NULL || !c->etat[i][j])
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
    }
    return voisins;

}

traj CalculTrajAvecFin(croisement* c, sommet debut, sommet fin, float v)
{
    croisement* current = c;
    file* closed = creer_file_vide();
    file* open = creer_file_vide();
    noeud* nDebut = malloc(sizeof(noeud));
    nDebut->s = debut;
    nDebut->cout = 0;
    nDebut->heuristic = distance3D(debut, fin);
    nDebut->v = v;
    nDebut->prec = NULL;
    ajouter_prio(open, nDebut);
    
    while(!file_est_vide(open))
    {
        if(current != NULL)
        {
            current = current->next;
        }

        noeud* u = defiler(open);
        ajouter_file(closed, u);

        if(memeSommet(u->s, fin))
        {
            printf("Trouver chemin\n");
            traj res = reconstituerTraj(debut, u, c);
            free_file(closed);
            free_file(open);
            return res;
        }
        if(u->s.z > fin.z)
        {
            free_file(closed);
            free_file(open);
            break;
        }
        file* voisins = prochainPoints(current, u);
        while(!file_est_vide(voisins))
        {
            noeud* v = defiler(voisins);
            v->cout = u->cout + distance3D(u->s, v->s);
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
    traj pasChemin;
    pasChemin.taille = 0;
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
    while(res.taille == 0)
    {
        sommet finTemps = {fin.x, fin.y, tempsEssayer};
        res = CalculTrajAvecFin(c, debut, finTemps, v);
        tempsEssayer = tempsEssayer + intervalleT;
    }
    return res;
}


int main()
{
    sommet debut = {50, 0, 0};
    position fin = {50, 99};
    croisement* c = NULL;
    traj Voiture1 = CalculTraj(c, debut, fin, 0);
    c = Voiture1.c;
    if(Voiture1.p != NULL)
    {
        afficher_pile(Voiture1.p);
        free_pile(Voiture1.p);
    }
    if(c != NULL)
    {
        free_croisement(c);
    }
    
}