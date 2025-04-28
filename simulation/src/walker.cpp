#include "include/walker.hpp"
#include <random>

using namespace std;

Location step(Graph& grid, float alpha) {
    random_device rd;
    default_random_engine eng(rd());
    uniform_real_distribution<float> distr(0, 1);

    if (distr(eng) > alpha)
    {
        // jump anywhere in the graph
    }
    else
    {
        // walk to a neighbor
        vector<Vertex> neighbors = grid.getNeighbours()
    }
}