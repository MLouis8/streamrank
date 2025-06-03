#include "include/network.hpp"
#include "include/rdLib.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <ostream>
#include <random>
#include <utility>
#include <vector>

Network::Network(int n, float p) {
  randomErdosRenyiNetwork(n, p);
  _n = n;
}

Network::Network(int n, float p, int vSumWt, int eSumWt, int nbEvents) {
  randomErdosRenyiNetwork(n, p);
  _n = n;
  for (int i = 0; i < n; i++)
    _nodeWeight[i] = 0;
  for (int j = 0; j < _adjacencyWeight.size(); j++)
    _adjacencyWeight[j] = 0;
  random_device rd;
  mt19937 gen(rd());
  _nodeWeight = rdTimeSeries(vSumWt, n, nbEvents);
  vector<int> weights(_adjacencyWeight.size(), 1);
  vector<int> edgeCap;
  for (int edge = 0; edge < _adjacencyWeight.size(); edge++) {
    pair<int, int> uv = getEdge(edge);
    edgeCap.push_back(min(_nodeWeight[uv.first], _nodeWeight[uv.second]));
  }
  discrete_distribution<> edgeDis(weights.begin(), weights.end());
  for (int y = 0; y < eSumWt; y++) {
    int id = edgeDis(gen);
    _adjacencyWeight[id] += 1;
    edgeCap[id]--;
    if (edgeCap[id] == 0) {
      weights[id] = 0;
      edgeDis = discrete_distribution<>(weights.begin(), weights.end());
    }
  }
}

vector<int> Network::neighbors(int u) {
  vector<int> neigh;
  for (int i = _xadjacency[u]; i < _xadjacency[u + 1]; i++)
    neigh.push_back(_adjacency[i]);
  return neigh;
}

vector<float> Network::neighborsWeights(int u) {
  vector<float> weights;
  for (int i = _xadjacency[u]; i < _xadjacency[u + 1]; i++)
    weights.push_back(_adjacencyWeight[i]);
  return weights;
}

pair<int, int> Network::getEdge(int uv) {
  int v = _adjacency[uv];
  int u = 0;
  for (int cumulDeg : _xadjacency) {
    if (cumulDeg > uv) {
      return {u - 1, v};
    }
    u++;
  }
  return {u - 1, v};
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
  cout << "Number of nodes: " << _n << " Number of edges: " << nbEdges() << "/"
       << nbEdges() / 2 << '\n';
  cout << "Cumulative nb of neighbor per node:";
  for (auto x : _xadjacency)
    cout << " " << x;
  cout << "\nWeights of the above nodes:";
  for (auto w : _nodeWeight)
    cout << " " << w;
  cout << "\nOrdered list of neighbors:";
  for (auto a : _adjacency)
    cout << " " << a;
  cout << "\nWeights of the above edges:";
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
  auto neighborConsistent = [&]() {
    for (int u = 0; u < _n; u++) {
      vector<int> neigh = neighbors(u);
      for (auto v : neigh) {
        vector<int> vNeigh = neighbors(v);
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