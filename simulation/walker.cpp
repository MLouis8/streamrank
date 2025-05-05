#include "include/walker.hpp"
#include "include/temporalNetwork.hpp"
#include <iostream>
#include <random>
#include <utility>

typedef std::pair<int, int> DTNode;
typedef std::pair<int, float> CTNode;

template <> int Walker<int>::step(Network &net, float alpha) {
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> jumpDis(0, 1);

  std::vector<int> neighbours = net.getNeighbours(_pos);

  if (jumpDis(eng) > alpha ||
      neighbours.size() == 0) { // jump anywhere in the graph
    _pos = net.getRdLocation(_id);
  } else { // walk to a neighbour
    std::vector<float> weights = net.getNeighboursWeights(_pos);
    std::discrete_distribution<> neighbourDis(weights.begin(), weights.end());
    _pos = neighbourDis(eng);
  }
  return _pos;
}

template <> DTNode Walker<DTNode>::step(tempoNetwork &tnet, float alpha) {
  return {0, 0};
}

template <> DTNode Walker<DTNode>::approxStep(tempoNetwork &tnet, float alpha) {
  return {0, 0};
}

template <>
CTNode Walker<CTNode>::continuousStep(tempoNetwork &tnet, float alpha) {
  return {0, 0.};
}