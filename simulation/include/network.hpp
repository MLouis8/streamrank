#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

class Network {
public:
  Network(std::vector<int> xadj, std::vector<int> adj, std::vector<float> adjWt)
      : _n(xadj.size() - 1), _xadjacency(xadj), _adjacency(adj),
        _adjacencyWeight(adjWt) {}
  std::vector<int> xAdjacency() { return _xadjacency; }
  std::vector<float> adjacencyWeight() { return _adjacencyWeight; }
  std::vector<int> adjacency() { return _adjacency; }
  int size() { return _n; }
  int nbEdges() { return _adjacencyWeight.size(); }

  std::vector<int> neighbours(int u);
  std::vector<float> neighboursWeights(int u);
  int getRdLocation(int u);
  void display();
  void checkConsistency();

private:
  int _n;
  std::vector<int> _xadjacency;
  std::vector<float> _adjacencyWeight;
  std::vector<int> _adjacency;
};

Network randomErdosRenyiNetwork(int n, float p);

#endif // NETWORK_HPP