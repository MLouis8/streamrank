#include <vector>

class Graph {
    size_t nbVertices;
    vector<size_t> adj;
    vector<size_t> 

    Graph()

    void addVertex(size_t u, size_t v);
};

Graph generateRDGraph(size_t n, float p, std::default_random_engine& eng);

vector<int> getNeighbours(size_t v);