#include "stdio.h"
#include "stdlib.h"
#include "chrono"
#include "SNL.h"
#include "mergeSort.h"

int main(int argc, char *argv[]) {

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    int n;
    fscanf(fp, "%d", &n);
    
    int *degrees = new int[n];
    SNL *nl = new SNL(n);

    for (int i = 0; i < n; i++) {
        degrees[i] = 0;
    }

    while(!feof(fp)) {
        int u, v;
        fscanf(fp, "%d %d", &u, &v);
        degrees[u]++;
        degrees[v]++;

        nl->push(u, v);
        nl->push(v, u);
    }

    // time start
    auto start = std::chrono::high_resolution_clock::now();

    //sort nodes_order by degree ascending, store transpose in nl->nodes_order
    mergeSort(degrees, nl->nodes_order, n);

    int misp_size = 0;
    int misp[n];
    
    //greedy MISP
    for (int i = 0; i < n; i++) {
        int node = nl->nodes_order[i];
        if (nl->valid[node]) {
            nl->valid[node] = false;
            misp[misp_size] = node;
            misp_size++;
            struct Neighbor* neighbor = nl->neighborhoods[node];
            while (neighbor != nullptr) {
                nl->valid[neighbor->node] = false;
                neighbor = neighbor->next;
            }
        }
    }

    //time end
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    printf("Time: %f ms\n", elapsed.count());

    fclose(fp);
    delete nl;
    return 0;
}