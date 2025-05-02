#include "include/walker.hpp"
#include <random>

template <> int Walker<int>::step(Network &net, float alpha) {
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> jumpDis(0, 1);

  if (jumpDis(eng) > alpha) { // jump anywhere in the graph
    _pos = net.getRdLocation();
  } else { // walk to a neighbor
    std::vector<int> neighbors = net.getNeighbours(_id);
    std::vector<float> weights = net.getNeighboursWeights(_id);
    std::discrete_distribution<> neighbourDis(weights.begin(), weights.end());
    _pos = neighbors[neighbourDis(eng)];
  }
  return _pos;
}