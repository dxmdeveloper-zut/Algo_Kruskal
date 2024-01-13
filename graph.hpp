#pragma once

#include <cstdlib>
#include <utility>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "union_find.hpp"
#include "vector.hpp"

#define HR_CLOCK_NOW std::chrono::high_resolution_clock::now()
#define HR_CLOCK_DURATION_NS std::chrono::duration_cast<std::chrono::nanoseconds>

namespace graphs {

    struct dbg_info {
        size_t find_calls_cnt = 0;
        std::chrono::nanoseconds sort_time;
        std::chrono::nanoseconds main_loop_time;
    };

    struct node {
        float x;
        float y;
    };

    struct edge {
        std::pair<size_t, size_t> node_ind;
        float cost = 0;
    };
}

class graph {
public:
    vec<graphs::node> vertices;
    vec<graphs::edge> edges;

public:
    graph(const std::string &filename);

    graph(std::istream &stream);

    graph(const graph &other);

    [[nodiscard]]
    vec<graphs::edge>
    Kruskal(graphs::dbg_info *measures, bool path_compression = true, bool unite_by_rank = true) const;

private:
    inline void read_graph(std::istream &stream);
};

graph::graph(const std::string &filename) {
    std::ifstream stream(filename);
    read_graph(stream);
}

graph::graph(std::istream &stream) {
    read_graph(stream);
}

inline void
graph::read_graph(std::istream &stream) {
    size_t vn, en;
    stream >> vn;
    vertices = vec<graphs::node>(vn);
    for (size_t i = 0; i < vn; i++) {
        graphs::node v{0};
        stream >> v.x;
        stream >> v.y;
        vertices[i] = v;
    }
    stream >> en;
    edges = vec<graphs::edge>(en);
    for (size_t i = 0; i < en; i++) {
        graphs::edge e;
        stream >> e.node_ind.first;
        stream >> e.node_ind.second;
        stream >> e.cost;
        edges[i] = e;
    }
    // sort edges by cost ascending
    std::sort(&edges[0], &edges[0] + edges.get_size(),
              [](graphs::edge &a, graphs::edge &b) {
                  return a.cost < b.cost;
              });
}

vec<graphs::edge> graph::Kruskal(graphs::dbg_info *measures, bool path_compression, bool unite_by_rank) const {
    vec<graphs::edge> forest(0);
    union_find dset(vertices.get_size(), path_compression, unite_by_rank);

    // sorting edges
    auto sort_start_time = HR_CLOCK_NOW;
    std::sort(&edges[0], &edges[0] + edges.get_size(),
              [](graphs::edge &a, graphs::edge &b) {
                  return a.cost < b.cost;
              });
    auto sort_end_time = HR_CLOCK_NOW;

    // Krukal's algorithm
    auto start_time = HR_CLOCK_NOW;
    std::for_each(&edges[0], &edges[0] + edges.get_size(), [&](graphs::edge &e) {
        if (dset.find(e.node_ind.first) != dset.find(e.node_ind.second)) {
            forest.push_back(e);
            dset.unite(dset.find(e.node_ind.first), dset.find(e.node_ind.second));
        }
    });
    auto end_time = HR_CLOCK_NOW;

    // Saving measures
    if (measures) {
        measures->find_calls_cnt = dset.find_calls_cnt;
        measures->sort_time = HR_CLOCK_DURATION_NS(sort_end_time - sort_start_time);
        measures->main_loop_time = HR_CLOCK_DURATION_NS(end_time - start_time);
    }
    return forest;
}

graph::graph(const graph &other) : vertices(other.vertices), edges(other.edges) {}
