#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

typedef struct sommet {
    float x;
    float y;
    float z;
} sommet;

typedef struct noeud {
    sommet s;
    float cout;
    float heuristic;
    float v;
    struct noeud* prec;
} noeud;

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


//----------GRAPHE---------
typedef struct tupleG {
    sommet p;
    sommet* arretes;
    int nombreArretes;
} tupleG;

typedef struct graphe {
    int nombreSommets;
    tupleG* liaisons;
} graphe;


//----------PILE-------------
typedef struct noeudPile {
    sommet val;
    struct noeudPile* next;
} noeudPile;

typedef struct pile {
    noeudPile* start;
} pile;

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
    //free(premier);
    return res;

}

sommet trouver(pile* p, int i)
{
    if(pile_est_vide(p))
    {
        printf("existe pas\n");
    }
    else
    {
        noeudPile* actual = p->start;
        for(int j = 0; j < i; i++)
        {
            actual = actual->next;
        }
        return actual->val;
    }
}



//-----------FILE--------------
typedef struct noeudFile {
    noeud val;
    struct noeudFile* next;
} noeudFile;

typedef struct file {
    noeudFile* first;
    noeudFile* last;
} file;

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

void ajouter_file(file* f, noeud n)
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

void ajouter_prio(file* f, noeud n)
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

        while(!(actual == NULL || n.heuristic < actual->val.heuristic))
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

bool appartient(file* f, noeud n)
{
    if(file_est_vide(f))
    {
        return false;
    }
    else
    {
        noeudFile* actual = f->first;
        while(!(actual == NULL || memeSommet(actual->val.s,n.s)))
        {
            actual = actual->next;
        }
        return !(actual == NULL);
    }
  
}

void mettreAJour(file* f, noeud n)
{
    if(!appartient(f, n))
    {
        ajouter_prio(f,n);
    }
    else
    {
        noeudFile* actual = f->first;
        noeudFile* actualPrec = NULL;
        while(!(actual == NULL || memeSommet(actual->val.s,n.s)))
        {
            actualPrec = actual;
            actual = actual->next;
        }
        if(actual->val.heuristic > n.heuristic)
        {
            if(actualPrec != NULL)
            {
                
                actualPrec->next = actual->next;
                ajouter_prio(f, n);
                free(actual);
            }
            else
            {
                printf("premier");
                actual->val = n;
            }
            
        }

    
    }
}

noeud defiler(file* f)
{
    if(!file_est_vide(f))
    {
        noeudFile* premier = f->first;
        noeud res = premier->val; 
        f->first = f->first->next;
        if(f->first == NULL)
        {
            f->last = NULL;
        }
        free(premier);
        return res;
    }
}



//-----------Algo------------------
typedef struct path {
    int taille;
    pile* p;
} path;

path reconstituerChemin(sommet fin, noeud debut)
{
    noeud actual = debut;
    int taille = 1;
    pile* p = creer_pile_vide();
    while(!memeSommet(actual.s, fin))
    {
        taille++;
        ajouter_pile(p, actual.s);
        actual = *(actual.prec);
    }
    path res = {taille, p};
    return res;
}

file* trouverVoisinsContraintes(graphe g, noeud u)
{
    file* voisins = creer_file_vide();
    for(int i = 0; i < g.nombreSommets; i++)
    {
        if(memeSommet(g.liaisons[i].p, u.s))
        {
            
            for(int j = 0; j < g.liaisons[i].nombreArretes; j++)
            {
                noeud new = {g.liaisons[i].arretes[j], 0, 0, 0, NULL};
                ajouter_file(voisins, new);
            }
            return voisins;

        }
    }

}

noeud Astar(graphe g, sommet debut, sommet fin, float v)
{
    
    file* closed = creer_file_vide();
    file* open = creer_file_vide();
    noeud nDebut = {debut, 0, distance3D(debut, fin), v, NULL};
    ajouter_prio(open, nDebut);
    
    while(!file_est_vide(open))
    {
        noeud u = defiler(open);
        if(u.prec != NULL)
        {
             printf("parent du suivant : %f, %f, %f\n\n", u.prec->s.x, u.prec->s.y, u.prec->s.z);
        }
       
        if(memeSommet(u.s, fin))
        {
            printf("Trouver chemin\n");
            //return reconstituerChemin(fin, u);
            return *u.prec;
        }
        ajouter_file(closed, u);
        file* voisins = trouverVoisinsContraintes(g, u);
       
        while(!file_est_vide(voisins))
        {
            printf("Exploration voisins\n\n");
            noeud v = defiler(voisins);
            v.cout = u.cout + distance3D(u.s, v.s);
            v.heuristic = v.cout + distance3D(v.s, fin);
            v.prec = &u;
            if(!appartient(closed, v))
            {
                printf("%f, %f, %f parent de : %f, %f, %f\n\n", v.prec->s.x, v.prec->s.y, v.prec->s.z, v.s.x, v.s.y, v.s.z);
                mettreAJour(open,v);
            }
        }
        

    }
    printf("Chemin non existant\n");

}




int main()
{
    sommet u = {0, 0, 0};
    sommet v = {1,1,0};
    sommet w = {3,1,0};
    sommet A = {3,-1,0};
    sommet B = {5,0,0};
    sommet C = {5,3,0};
    sommet D = {8,1,0};

    int nombreArretesU = 1;
    sommet arretesU[1]  = {v};
    tupleG tupleU = {u, arretesU, nombreArretesU};

    int nombreArretesV = 3;
    sommet arretesV[3]  = {u,w,A};
    tupleG tupleV = {v, arretesV, nombreArretesV};

    int nombreArretesW = 3;
    sommet arretesW[3]  = {v,B,C};
    tupleG tupleW = {w, arretesW, nombreArretesW};

    int nombreArretesA = 2;
    sommet arretesA[2]  = {v,B};
    tupleG tupleA = {A, arretesA, nombreArretesA};

    int nombreArretesB = 3;
    sommet arretesB[3]  = {w,A,D};
    tupleG tupleB = {B, arretesB, nombreArretesB};

    int nombreArretesC = 2;
    sommet arretesC[2]  = {w,D};
    tupleG tupleC = {C, arretesC, nombreArretesC};

    int nombreArretesD = 2;
    sommet arretesD[2]  = {B,C};
    tupleG tupleD = {D, arretesD, nombreArretesD};

    tupleG liaisonG[7] = {tupleA,tupleB,tupleC,tupleD,tupleW,tupleU,tupleV};

    graphe g = {7, liaisonG};

    noeud nu = {w, 0,0,0,NULL};

    file* voisins = trouverVoisinsContraintes(g, nu);
    /*
    while(!file_est_vide(voisins))
    {
        noeud actuel = defiler(voisins);
        printf("Actuel : x : %f, y : %f, z : %f\n", actuel.s.x, actuel.s.y, actuel.s.z);
        printf("\n");
    }*/
    
    noeud path = Astar(g, u, D, 0);

    printf("suivant : %f, %f, %f\n\n", path.prec->s.x, path.prec->s.y, path.prec->s.z);
/*
    noeud* actual = &path;
    while(actual != NULL)
    {
        printf("Actuel : x : %f, y : %f, z : %f\n", actual->s.x, actual->s.y, actual->s.z);
        actual = actual->prec;
    }*/
  
    //path plusCourtChemin = Astar(g, u, D, 0);
    //depiler(plusCourtChemin.p);
    /*
    for(int i = 0; i < plusCourtChemin.taille; i++)
    {
        sommet actual = trouver(plusCourtChemin.p, i);
        printf("x : %f, y : %f, z : %f\n", actual.x, actual.y, actual.z);
    }*/
    

}