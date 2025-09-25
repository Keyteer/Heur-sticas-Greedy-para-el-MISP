#pragma once

struct Neighbor {
    int node;
    struct Neighbor *next;
};

// Neighborhood List
struct NeighList {
    int n;
    int *degrees;
    struct Neighbor **neighborhoods;

    NeighList(int n) {
        this->n = n;
        degrees = new int[n];
        neighborhoods = new struct Neighbor *[n];
        for (int i = 0; i < n; i++) {
            degrees[i] = 0;
            neighborhoods[i] = nullptr;
        }
    }
    ~NeighList() {
        delete[] degrees;
        for (int i = 0; i < n; i++) {
            struct Neighbor *current = neighborhoods[i];
            while (current != nullptr) {
                struct Neighbor *temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] neighborhoods;
    }

    void push(int u, int v) {
        struct Neighbor *newNeighbor = new struct Neighbor();
        newNeighbor->node = v;
        newNeighbor->next = nullptr;

        if (neighborhoods[u] == nullptr) {
            neighborhoods[u] = newNeighbor;
        } else {
            struct Neighbor *temp = neighborhoods[u];
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNeighbor;
        }

        degrees[u]++;
    }
};