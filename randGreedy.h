#include "stdio.h"
#include "stdlib.h"
#include "NeighList.h"

int randGreedy(int n, NeighList *nl){
    int misp_size = 0;
    
    int cont = n;
    bool *visited = new bool[n];
    for (int i = 0; i < n; ++i)
        visited[i] = false;

    int node = rand() % n;
    misp_size++;
    cont--;
    visited[node] = true;

    //random greedy MISP
    while (0<=cont){
        vector<int>::iterator neighbor = nl->neighborhoods[node].begin();
        while (neighbor != nl->neighborhoods[node].end() && visited[*neighbor] == true) {
            neighbor++;
        }
        if (neighbor != nl->neighborhoods[node].end()){
            visited[*neighbor] = true;
            cont--;
            vector<int>::iterator temp = neighbor + 1;
            while (temp != nl->neighborhoods[node].end()){
                visited[*temp] = true;
                cont--;
                temp++;
            }
        }
        
        int minDeg = n;
        int temNode = node;
        while (neighbor != nl->neighborhoods[node].end()) {
            if (visited[*neighbor]==false && nl->degrees[*neighbor]<minDeg){
                minDeg = nl->degrees[*neighbor];
                temNode = *neighbor;
            }
            neighbor++;
        }
        if (node != temNode){
            node = temNode;
        }else{
            if (0<cont){
               node = rand() % n;
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