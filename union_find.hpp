#pragma once

#include "vector.hpp"

class union_find {
private:
    vec<size_t> parent;
    vec<uint> rank; // górne ograniczenia na wysokości drzew
    bool path_compression = true;
    bool unite_by_rank = true;

public:
    size_t find_calls_cnt = 0;

public:
    union_find(size_t n, bool path_compression = true, bool unite_by_rank = true);

    size_t find(size_t x);

    void unite(size_t x, size_t y);
};

union_find::union_find(size_t n, bool path_compression, bool unite_by_rank)
        : parent(n), rank(n), path_compression(path_compression), unite_by_rank(unite_by_rank) {
    for (size_t i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}

void union_find::unite(size_t x, size_t y) {
    size_t x_root = find(x);
    size_t y_root = find(y);

    if (!unite_by_rank) {
        if (x_root != y_root)
            parent[y_root] = x_root;
        return;
    }
    if (rank[x_root] > rank[y_root]) {
        parent[y_root] = x_root;
    } else if (rank[x_root] < rank[y_root]) {
        parent[x_root] = y_root;
    } else if (x_root != y_root) {
        parent[y_root] = x_root;
        rank[x_root] += 1;
    }

}


size_t union_find::find(size_t x) {
    find_calls_cnt++;
    if (parent[x] == x) return x;
    if (path_compression) {
        return (parent[x] = find(parent[x]));
    }
    return find(parent[x]);
}

