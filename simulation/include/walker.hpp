#ifndef WALKER_HPP
#define WALKER_HPP

#include "network.hpp"
#include "temporalNetwork.hpp"

typedef std::pair<int, int> DTNode;

template <typename Location> class Walker {
public:
  Walker(int id, Location startingPos) {
    _id = id;
    _pos = startingPos;
  }
  int getId() const { return _id; }
  Location getLoc() { return _pos; }
  /**
   * Function executing the step of a pagerank simulation
   */
  Location step(Network &net, float alpha);
  /**
   * Function executing the step of the DTRW
   */
  Location step(tempoNetwork &tnet, float alpha);
  /**
   * Function executing the step of the approximation of the DTRW
   */
  Location approxStep(tempoNetwork &tnet, float alpha);
  /**
   * Function executing the step corresponding to the upper bound of the DTRW
   */
  Location upperBound(tempoNetwork &tnet, float alpha);
  /**
   * Function executing the step corresponding to the lower bound of the DTRW
   */
  Location lowerBound(tempoNetwork &tnet, float alpha);

private:
  int _id;
  Location _pos;
};

#endif // WALKER_HPP