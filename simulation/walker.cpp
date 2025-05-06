#include "include/walker.hpp"
#include "include/temporalNetwork.hpp"
#include <random>
#include <utility>
#include <vector>

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
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::vector<std::tuple<int, int, int>> fneighbours =
      tnet.getFutureNeighbours(_pos.first, _pos.second);
  std::vector<float> w;
  for (auto tup : fneighbours) {
    w.push_back(std::get<2>(tup) - std::get<1>(tup));
  }
  std::discrete_distribution<> dis(w.begin(), w.end());
  int v = std::get<0>(fneighbours[dis(eng)]);
}

template <>
DTNode Walker<DTNode>::upperBound(tempoNetwork &tnet, float alpha) {}

template <>
DTNode Walker<DTNode>::lowerBound(tempoNetwork &tnet, float alpha) {}