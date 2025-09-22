
struct Neighbor {
    int node;
    struct Neighbor* next;
};

// Sorted Neighborhood List
struct SNL {
    int n;
    int* nodes_order;
    struct Neighbor** neighborhoods;
    
    bool* valid;

    SNL(int n) {
        this->n = n;
        nodes_order = new int[n];
        neighborhoods = new struct Neighbor*[n];
        valid = new bool[n];
        for (int i = 0; i < n; i++) {
            nodes_order[i] = i;
            neighborhoods[i] = nullptr;
            valid[i] = true;
        }
    }
    ~SNL() {
        delete[] nodes_order;
        delete[] valid;
        for (int i = 0; i < n; i++) {
            struct Neighbor* current = neighborhoods[i];
            while (current != nullptr) {
                struct Neighbor* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] neighborhoods;
    }

    void push(int u, int v) {
        struct Neighbor* newNeighbor = new struct Neighbor();
        newNeighbor->node = v;
        newNeighbor->next = nullptr;

        if (neighborhoods[u] == nullptr) {
            neighborhoods[u] = newNeighbor;
        } else {
            struct Neighbor* temp = neighborhoods[u];
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNeighbor;
        }
    }

};