#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

class Network
{
public:
    Network(std::vector<int> xadj, std::vector<int> adj, std::vector<float> adjWt) : _n(xadj.size()-1), _xadjacency(xadj), _adjacency(adj), _adjacencyWeight(adjWt) {}
    std::vector<int> getXadjacency() { return _xadjacency; }
    std::vector<float> getAdjacencyWeight() { return _adjacencyWeight; }
    std::vector<int> getAdjacency() { return _adjacency; }
    int getSize() { return _n; }
    
    std::vector<int> getNeighbours(int u);
    std::vector<float> getNeighboursWeights(int u);
    int getRdLocation();

private:
    int _n;
    std::vector<int> _xadjacency;
    std::vector<float> _adjacencyWeight;
    std::vector<int> _adjacency;
};

Network randomErdosRenyiNetwork(int n, float p);

#endif // NETWORK_HPP