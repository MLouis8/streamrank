#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

class Network
{
public:
    Network(std::vector<int> xadj, std::vector<float> adjWt, std::vector<int> adj) : _xadjacency(xadj), _adjacencyWeight(adjWt), _adjacency(adj) {}
    std::vector<int> getNeighbours(int u);
    std::vector<float> getNeighboursWeights(int u);
    int getRdLocation();

private:
    int _n;
    std::vector<int> _xadjacency;
    std::vector<float> _adjacencyWeight;
    std::vector<int> _adjacency;
};

Network &randomErdosRenyiNetwork(int n, float p);

#endif // NETWORK_HPP