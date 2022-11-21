#include "AStar.h"

void afficher_sommet(sommet s)
{
    printf("%d %d %f\n", s.x, s.y, s.z);
}

//--------Comparaison Sommets-----------
bool memeSommet(sommet A, sommet B)
{
    return A.x == B.x && A.y == B.y && A.z == B.z;
}

float distance3D(sommet A, sommet B)
{
    float ux = B.x - A.x;
    float uy = B.y - A.y;
    float uz = B.z - A.z;
    return sqrt(pow(ux, 2) + pow(uy,2) + pow(uz,2));
}

//----------PILE-------------
pile* creer_pile_vide()
{
    pile* new = malloc(sizeof(pile));
    new->start = NULL;
    return new;
}

bool pile_est_vide(pile* p)
{
    return p->start == NULL;
}

void ajouter_pile(pile* p, sommet A)
{
    noeudPile* ajout = malloc(sizeof(noeudPile));
    ajout->val = A;
    ajout->next = p->start;
    p->start = ajout;
}

sommet depiler(pile* p)
{
    noeudPile* premier = p->start;
    sommet res = premier->val;
    p->start = p->start->next;
    free(premier);
    return res;

}

void free_pile(pile* p)
{
    noeudPile* current = p->start;
    while(current!= NULL)
    {
        noeudPile* next = current->next;
        free(current);
        current = next;
    }
    free(p);
}

void afficher_pile(pile* p)
{
    if(p == NULL)
    {
        return;
    }
    noeudPile* current = p->start;
    while(current!= NULL)
    {
        afficher_sommet(current->val);
        current = current->next;
        printf("\n");
    }

}



//-----------FILE--------------
file* creer_file_vide()
{
    file* new = malloc(sizeof(file));
    new->first = NULL;
    new->last = NULL;
    return new;
}

bool file_est_vide(file* f)
{
    return f->first == NULL;
}

void ajouter_file(file* f, noeud* n)
{
    noeudFile* ajout = malloc(sizeof(noeudFile));
    ajout->val = n;
    ajout->next = NULL;
    if(file_est_vide(f))
    {
        f->first = ajout;
        f->last = ajout;
    }
    else
    {
        f->last->next = ajout;
        f->last = ajout;
    }
}

void ajouter_prio(file* f, noeud* n)
{
    if(file_est_vide(f))
    {
        ajouter_file(f, n);
    }
    else
    {
        noeudFile* actual = f->first;
        noeudFile* actualPrec = NULL;

        noeudFile* ajout = malloc(sizeof(noeudFile));
        ajout->val = n;

        while(!(actual == NULL || n->heuristic < actual->val->heuristic))
        {
            actualPrec = actual;
            actual = actual->next;  
        }
        ajout->next = actual;
        if(actualPrec == NULL)
        {
            f->first = ajout;
        }
        else
        {
            actualPrec->next = ajout;
        }
    }

}

bool appartient(file* f, noeud* n)
{
    if(file_est_vide(f))
    {
        return false;
    }
    else
    {
        noeudFile* actual = f->first;
        while(!(actual == NULL || memeSommet(actual->val->s,n->s)))
        {
            actual = actual->next;
        }
        return !(actual == NULL);
    }
  
}

void mettreAJour(file* f, noeud* n)
{
    if(file_est_vide(f))
    {
        ajouter_file(f,n);
    }
    else
    {
        noeudFile* actual = f->first;
        while(!(actual == NULL || memeSommet(actual->val->s,n->s)))
        {
            actual = actual->next;
        }
        if(actual == NULL)
        {
            ajouter_file(f,n);
        }
        else if(actual->val->heuristic > n->heuristic)
        {
            free(actual->val);
            actual->val = n;
        }
        else
        {
            free(n);
        }

    
    }
}

noeud* defiler(file* f)
{
    if(!file_est_vide(f))
    {
        noeudFile* premier = f->first;
        noeud* res = premier->val; 
        f->first = f->first->next;
        if(f->first == NULL)
        {
            f->last = NULL;
        }
        free(premier);
        return res;
    }
    else
    {
        return NULL;
    }
}

noeud* defiler_prio(file* f)
{
    if(!file_est_vide(f))
    {
        noeudFile* current = f->first;
        noeudFile* prevMin = NULL;
        noeudFile* min = f->first;
        while(current->next != NULL)
        {
            if(current->next->val->heuristic < min->val->heuristic)
            {
                prevMin = current;
                min = current->next;
            }
            current = current->next;
        }
        if(prevMin != NULL)
        {
            noeud* res = min->val;
            prevMin->next = min->next;
            if(prevMin->next == NULL)
            {
                f->last = prevMin;
            }
            free(min);
            return res;
        }
        else
        {
            noeud* res = min->val;
            f->first = min->next;
            free(min);
            return res;
        }
    }
    else
    {
        return NULL;
    }
}

void free_file(file* f)
{
    noeudFile* current = f->first;
    while(current!= NULL)
    {
        noeudFile* next = current->next;
        free(current->val);
        free(current);
        current = next;
    }
    free(f);
}

//-----------Algo------------------
path reconstituerChemin(sommet debut, noeud* fin)
{
    noeud* actual = fin;
    int taille = 1;
    pile* p = creer_pile_vide();
    while(!memeSommet(actual->s, debut))
    {
        taille++;
        ajouter_pile(p, actual->s);
        actual = (actual->prec);
    }
    ajouter_pile(p, debut);
    path res = {taille, p};
    return res;
}

file* trouverVoisins(graphe* g, noeud* u)
{
    file* voisins = creer_file_vide();
    for(int i = 0; i < g->nombreSommets; i++)
    {
        if(memeSommet(g->liaisons[i].p, u->s))
        {
            
            for(int j = 0; j < g->liaisons[i].nombreArretes; j++)
            {
                noeud* new = malloc(sizeof(noeud));
                new->s = g->liaisons[i].arretes[j];
                new->cout = 0;
                new->heuristic = 0;
                new->v = 0;
                new->prec = NULL;
                ajouter_file(voisins, new);
            }
        }
    }
    return voisins;

}

path Astar(graphe* g, sommet debut, sommet fin, float v)
{  
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
        noeud* u = defiler(open);
        ajouter_file(closed, u);

        if(memeSommet(u->s, fin))
        {
            printf("Trouver chemin\n");
            path res = reconstituerChemin(debut, u);
            free_file(closed);
            free_file(open);
            return res;
        }

        file* voisins = trouverVoisins(g, u);
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
    path pasChemin;
    pasChemin.taille = 0;
    pasChemin.p = NULL;
    printf("Chemin non existant\n");
    return pasChemin;

}
