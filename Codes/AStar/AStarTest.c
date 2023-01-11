#include "AStar.h"


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

    path plusCourt = Astar(&g, u, D, 0);
    afficher_pile(plusCourt.p, 0, "AStarTest");
    free_pile(plusCourt.p);

}