#include "AStar/AStar.h"

typedef bool intersection[200][200];

typedef struct croisement {
    intersection etat;
    int t0;
    struct croisement* next;
} croisement;

typedef struct position {
    float x;
    float y;
} position;

typedef position* trajectoire;

graphe creerGraphe(croisement* c)
{
    graphe g;
    return g;
}

trajectoire calculTraj(croisement* c)
{
    trajectoire res;
    return res;
}

void miseAJourCroisement(croisement* c)
{
    if(c != NULL)
    {
        croisement* aLib = c;
        c = c->next;
        free(aLib);
    }
}

int main()
{
    while(true)
    {
        printf("Hello World\n");
    }

}