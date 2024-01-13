#include <iostream>
#include <numeric>
#include "graph.hpp"
#define VERBOSE 1

inline void print_measures(const graphs::dbg_info &measures) {
    std::cout << "find operations: " << measures.find_calls_cnt << std::endl;
    std::cout << "sort time: " << measures.sort_time.count() << "ns" << std::endl;
    std::cout << "algorithm's time: " << measures.main_loop_time.count() << "ns" << std::endl;
}

inline void print_mst_info(const vec<graphs::edge> &mst) {
    std::cout << "MST total cost: "
              << std::accumulate(&mst[0], &mst[0] + mst.get_size(), 0.0f,
                                 [](float acc, graphs::edge &e) { return acc + e.cost; }) << std::endl
              << "MST edge count: " << mst.get_size() << std::endl;
#ifdef VERBOSE
    std::cout << "MST edges: " << std::endl;
    for (size_t i = 0; i < mst.get_size(); i++) {
        std::cout << "(" << mst[i].node_ind.first << ", " << mst[i].node_ind.second << "), cost: " << mst[i].cost << std::endl;
    }
#endif
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Expected filename of file with graph as program argument";
        return 1;
    }
    struct graphs::dbg_info measures;
    graph graph_obj((std::string(argv[1])));

    auto mst = graph_obj.Kruskal(&measures, true, true);
    print_mst_info(mst);

    std::cout << "\nKruskal with path compression and unite by rank" << std::endl;
    print_measures(measures);

    graph_obj.Kruskal(&measures, false, true);
    std::cout << "\nKruskal without path compression and with unite by rank" << std::endl;
    print_measures(measures);

    graph_obj.Kruskal(&measures, true, false);
    std::cout << "\nKruskal with path compression and without unite by rank" << std::endl;
    print_measures(measures);

    graph_obj.Kruskal(&measures, false, false);
    std::cout << "\nKruskal without path compression and without unite by rank" << std::endl;
    print_measures(measures);

    return 0;
}
