#include "stdio.h"
#include "stdlib.h"
#include "NeighList.h"

int randGreedy(int n, NeighList *nl){
    int misp_size = 0;
    
    int cont = n;
    bool *visited = new bool[n];
    for (int i = 0; i < n; ++i)
        visited[i] = false;

    int node = nl->neighborhoods[rand() % n]->node;
    misp_size++;
    cont--;
    visited[node] = true;

    //random greedy MISP
    while (0<=cont){
        struct Neighbor* neighbor = nl->neighborhoods[node];
        visited[neighbor->node] = true;
        cont--;

        int minDeg = n;
        int temNode = node;
        while (neighbor != nullptr) {
            if (visited[neighbor->node]==false && nl->degrees[neighbor->node]<minDeg){
                minDeg = nl->degrees[neighbor->node];
                temNode = neighbor->node;
            }
            neighbor = neighbor->next;
        }
        if (node != temNode){
            node = temNode;
        }else{
            if (0<cont){
               node = nl->neighborhoods[rand() % n]->node;
                while (visited[node]==true){
                    node = nl->neighborhoods[rand() % n]->node;
                }
            }else{
                break;
            }
        }
        misp_size++;
        cont--;
        visited[node] = true;
    }
    return misp_size;
}