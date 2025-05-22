#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

using namespace std;

/**
 * Network class, a graph stored with adjacency lists.
 */
class Network {
public:
  Network(int n, float p);
  Network(vector<int> xadj, vector<int> adj, vector<int> adjWt,
          vector<int> ndWt)
      : _n(xadj.size() - 1), _xadjacency(xadj), _adjacency(adj),
        _adjacencyWeight(adjWt), _nodeWeight(ndWt) {}
  vector<int> xAdjacency() { return _xadjacency; }
  vector<int> adjacencyWeight() { return _adjacencyWeight; }
  vector<int> adjacency() { return _adjacency; }
  int size() { return _n; }
  int nbEdges() { return _adjacencyWeight.size(); }

  void randomErdosRenyiNetwork(int n, float p);

  vector<int> neighbours(int u);
  vector<float> neighboursWeights(int u);
  int getRdLocation(int u);
  void display();
  void checkConsistency();

private:
  int _n;
  vector<int> _xadjacency;
  vector<int> _adjacencyWeight;
  vector<int> _adjacency;
  vector<int> _nodeWeight;
};

#endif // NETWORK_HPP