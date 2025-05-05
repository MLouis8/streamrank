#include "include/network.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <ostream>
#include <random>
#include <vector>

std::vector<int> Network::getNeighbours(int u) {
  std::vector<int> neighbours;
  for (int i = _xadjacency[u]; i < _xadjacency[u + 1]; i++)
    neighbours.push_back(_adjacency[i]);
  return neighbours;
}

std::vector<float> Network::getNeighboursWeights(int u) {
  std::vector<float> weights;
  for (int i = _xadjacency[u]; i < _xadjacency[u + 1]; i++)
    weights.push_back(_adjacencyWeight[i]);
  return weights;
}

int Network::getRdLocation(int u) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, _n - 1);
  int dest = dis(gen);
  while (dest == u)
    dest = dis(gen);
  return dest;
}

void Network::display() {
  std::cout << "Number of nodes: " << _n << std::endl;
  std::cout << "Cumulative nb of neighbour per node:";
  for (auto x : _xadjacency) {
    std::cout << " " << x;
  }
  std::cout << std::endl << "Ordered list of neighbours:";
  for (auto a : _adjacency) {
    std::cout << " " << a;
  }

  std::cout << std::endl << "Weight of the above edges:";
  for (auto w : _adjacencyWeight) {
    std::cout << " " << w;
  }
}

void Network::checkConsistency() {
  assert(_n == _xadjacency.size() - 1);
  assert(getNbEdges() == _adjacency.size());
  assert(_xadjacency.back() == getNbEdges());
  assert(_xadjacency[0] == 0);
  auto isSorted = [&](std::vector<int> v) {
    for (int i = 1; i < v.size(); i++)
      if (v[i] < v[i - 1])
        return false;
    return true;
  };
  assert(isSorted(_xadjacency));
  auto neighbourConsistent = [&]() {
    for (int u = 0; u < _n; u++) {
      std::vector<int> neighbours = getNeighbours(u);
      for (auto v : neighbours) {
        std::vector<int> neighbourNeighbours = getNeighbours(v);
        int occ = std::count(neighbourNeighbours.begin(),
                             neighbourNeighbours.end(), u);
        if (occ != 1)
          return false;
      }
    }
    return true;
  };
}

Network randomErdosRenyiNetwork(int n, float p) {
  std::vector<int> xadjacency;
  std::vector<float> adjacencyWeight;
  std::vector<int> adjacency;

  xadjacency.assign(n + 1, 0);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  std::map<std::pair<int, int>, bool> notSet;
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
  adjacencyWeight.assign(adjacency.size(), 1);
  return Network(xadjacency, adjacency, adjacencyWeight);
}