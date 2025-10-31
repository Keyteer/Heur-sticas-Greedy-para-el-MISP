#include "stdio.h"
#include "stdlib.h"
#include "NeighList.h"

int randGreedy(int n, NeighList *nl){
    int misp_size = 0;
    
    int cont = n;
    bool *visited = new bool[n];
    for (int i = 0; i < n; ++i)
        visited[i] = false;

    int node = nl->neighborhoods[rand() % n]->node();
    misp_size++;
    cont--;
    visited[node] = true;

    //random greedy MISP
    while (0<=cont){
        struct Neighbor* neighbor = nl->neighborhoods[node];
        while (neighbor != nullptr && visited[neighbor->node()] == true) {
            neighbor = neighbor->next();
        }
        if (neighbor != nullptr){
            visited[neighbor->node()] = true;
            cont--;
            struct Neighbor* temp = neighbor->next();
            while (temp != nullptr){
                visited[temp->node()] = true;
                cont--;
                temp = temp->next();
            }
        }
        
        int minDeg = n;
        int temNode = node;
        while (neighbor != nullptr) {
            if (visited[neighbor->node()]==false && nl->degrees[neighbor->node()]<minDeg){
                minDeg = nl->degrees[neighbor->node()];
                temNode = neighbor->node();
            }
            neighbor = neighbor->next();
        }
        if (node != temNode){
            node = temNode;
        }else{
            if (0<cont){
               node = nl->neighborhoods[rand() % n]->node();
                while (visited[node]==true){
                    node = (node + 1)%n;
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