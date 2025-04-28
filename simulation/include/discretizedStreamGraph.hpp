#include <vector>
#include <pair>

using pair<size_t, size_t> as Vertex;

class StreamGraph {
    size_t nbVertices;
    float T;
    vector<vector<pair<float, float>>> W;
    vector<vector<pair<float, float>>> E;
};

vector<Vertex> getNeighbours(Vertex v);