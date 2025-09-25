#include "stdio.h"
#include "stdlib.h"
#include "NeighList.h"
#include "mergeSort.h"

// greedy MISP
// n [in]:        number of nodes
// nl [in]:       neighborhood list
// misp [out]:    array to store the MISP nodes
// returns:       the size of the MISP
int gready(int n, NeighList *nl, int *misp) {
    int misp_size = 0;

    // initialize nodes_order with identity mapping
    int *nodes_order = new int[n];
    for (int i = 0; i < n; ++i)
        nodes_order[i] = i;

    // make a copy of degrees in NeighList to avoid modifying the original
    int *degrees = new int[n];
    for (int i = 0; i < n; ++i)
        degrees[i] = nl->degrees[i];

    //sort nodes_order by degree ascending, store transpose in nodes_order
    mergeSort(degrees, nodes_order, n);

    bool *visited = new bool[n];
    for (int i = 0; i < n; ++i)
        visited[i] = false;

    //greedy MISP
    for (int i = 0; i < n; i++) {
        int node = nodes_order[i];
        if (!visited[node]) {
            visited[node] = true;
            misp[misp_size] = node;
            misp_size++;
            struct Neighbor *neighbor = nl->neighborhoods[node];
            while (neighbor != nullptr) {
                visited[neighbor->node] = true;
                neighbor = neighbor->next;
            }
        }
    }

    delete[] nodes_order;
    delete[] degrees;

    return misp_size;
}