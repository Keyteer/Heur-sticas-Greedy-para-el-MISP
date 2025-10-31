#pragma once

#include <vector>
using std::vector;

// mantain Neighbor struct for easy backward compatibility
struct Neighbor : vector<int>::iterator {
    int node(){ return **this; }
    struct Neighbor *next(){ return this + 1; }
};

// Neighborhood List
struct NeighList {
    int n;
    int *degrees;
    struct Neighbor **neighborhoods;
    vector<int> *neighborhood_vectors;

    NeighList(int n) {
        this->n = n;
        degrees = new int[n];
        neighborhood_vectors = new vector<int>[n];
        neighborhoods = new struct Neighbor *[n];
        for (int i = 0; i < n; i++) {
            degrees[i] = 0;
            neighborhoods[i] = nullptr;
        }
    }
    ~NeighList() {
        delete[] degrees;
        delete[] neighborhoods;
    }

    void push(int u, int v) {
        neighborhood_vectors[u].push_back(v);
        degrees[u]++;

        if (neighborhoods[u] == nullptr) {
            neighborhoods[u] = (struct Neighbor *)&neighborhood_vectors[u].begin();
        }
    }

    bool isNeighbor(int u, int v) {
        for (int neighbor : neighborhood_vectors[u]) {
            if (neighbor == v) {
                return true;
            }
        }
        return false;
    }
};