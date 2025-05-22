#include "include/network.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <ostream>
#include <random>
#include <vector>

Network::Network(int n, float p) {
  randomErdosRenyiNetwork(n, p);
  _n = n;
}

vector<int> Network::neighbours(int u) {
  vector<int> neigh;
  for (int i = _xadjacency[u]; i < _xadjacency[u + 1]; i++)
    neigh.push_back(_adjacency[i]);
  return neigh;
}

vector<float> Network::neighboursWeights(int u) {
  vector<float> weights;
  for (int i = _xadjacency[u]; i < _xadjacency[u + 1]; i++)
    weights.push_back(_adjacencyWeight[i]);
  return weights;
}

int Network::getRdLocation(int u) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, _n - 1);
  int dest = dis(gen);
  while (dest == u)
    dest = dis(gen);
  return dest;
}

void Network::display() {
  cout << "Number of nodes: " << _n << endl;
  cout << "Cumulative nb of neighbour per node:";
  for (auto x : _xadjacency)
    cout << " " << x;
  cout << endl << "Ordered list of neighbours:";
  for (auto a : _adjacency)
    cout << " " << a;
  cout << endl << "Weight of the above edges:";
  for (auto w : _adjacencyWeight)
    cout << " " << w;
}

void Network::checkConsistency() {
  assert(_n == _xadjacency.size() - 1);
  assert(nbEdges() == _adjacency.size());
  assert(_xadjacency.back() == nbEdges());
  assert(_xadjacency[0] == 0);
  auto isSorted = [&](vector<int> v) {
    for (int i = 1; i < v.size(); i++)
      if (v[i] < v[i - 1])
        return false;
    return true;
  };
  assert(isSorted(_xadjacency));
  auto neighbourConsistent = [&]() {
    for (int u = 0; u < _n; u++) {
      vector<int> neigh = neighbours(u);
      for (auto v : neigh) {
        vector<int> vNeigh = neighbours(v);
        int occ = count(vNeigh.begin(), vNeigh.end(), u);
        if (occ != 1)
          return false;
      }
    }
    return true;
  };
}

void Network::randomErdosRenyiNetwork(int n, float p) {
  vector<int> xadjacency(n + 1, 0);
  vector<int> adjacency;

  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(0.0, 1.0);

  map<pair<int, int>, bool> notSet;
  for (int i = 0; i < n; i++) {
    int x = 0;
    for (int j = 0; j < n; j++) {
      if (i < j) {
        if (dis(gen) <= p) {
          adjacency.push_back(j);
          notSet[{j, i}] = true;
          x++;
        } else {
          notSet[{j, i}] = false;
        }
      } else if (i != j && notSet[{i, j}]) {
        adjacency.push_back(j);
        x++;
      }
    }
    xadjacency[i + 1] = xadjacency[i] + x;
  }
  vector<int> adjacencyWeight(adjacency.size(), 1);
  vector<int> ndWeights(n, 1);
  _xadjacency = xadjacency;
  _adjacency = adjacency;
  _adjacencyWeight = adjacencyWeight;
  _nodeWeight = ndWeights;
}