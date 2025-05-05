#ifndef TEMPORALNETWORK_HPP
#define TEMPORALNETWORK_HPP

#include "event.hpp"
#include <cmath>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

typedef std::vector<std::pair<float, float>> TIntervals;

class tempoNetwork {
public:
  tempoNetwork(float tStart, float tEnd, int n, std::vector<TIntervals> W,
               std::unordered_map<std::string, TIntervals> E)
      : _tStart(tStart), _tEnd(tEnd), _n(n), _W(W), _E(E),
        _timeAspectSet(false) {}
  float getStartingT() { return _tStart; }
  float getEndingT() { return _tEnd; }
  int getSize() { return _n; }
  TIntervals getTIntervals(int u) { return _W[u]; }
  TIntervals getTIntervals(int u, int v);
  bool isTimeSet() { return _timeAspectSet; }

  /**
   * Instantiate tempoNetwork class attributes linked to time: _events,
   * _nodeEvents, _edgeEvents necessary for accessing future neighbourhoods and
   * vanishing or appearance times.
   */
  void initTimeEvents();

  // Takes a time instant and returns the id of the event just before it
  int timeToEventId(float t);
  bool checkNodePres(int u, float t);
  bool checkEdgePres(int u, int v, float t);
  float getNodeVanishT(int u, float t);
  float getNodeAppearT(int u, float t);

  /**
   * Returns the vector of node ids neighbours of u at the event time
   * corresponding to t
   */
  std::vector<int> getInstantNeighbours(int u, float t);
  /**
   * Returns a vector of tuples of int, float, float containing [nodeId,
   * appearance time, vanishing time]
   */
  std::vector<std::tuple<int, float, float>> getFutureNeighbours(int u,
                                                                 float t);
  /**
   * Returns a vector of tuples of int containing [nodeId, appearance event id,
   * vanishing event id]
   */
  std::vector<std::tuple<int, int, int>> getFutureNeighbours(int u,
                                                             int idEvent);
  int getNodeVanishEventId(int u, int k);
  int getNodeAppearEventId(int u, int k);

private:
  float _tStart;
  float _tEnd;
  int _n;
  std::vector<TIntervals> _W;
  std::unordered_map<std::string, TIntervals> _E;

  bool _timeAspectSet;
  std::vector<Event> _events;
  std::vector<std::vector<int>> _nodeEvents;
  std::vector<std::vector<std::pair<int, int>>> _edgeEvents;
};

/**
 * Class defining a simple custom exception, alerting on the lack of temporal
 * aspect initialization, can be solve by calling initTimeEvents()
 */
class missing_temporal_init : public std::exception {
private:
  std::string message;

public:
  missing_temporal_init(const char *msg) : message(msg) {}
  const char *what() const throw() { return message.c_str(); }
};

bool timeValid(TIntervals, float t);
tempoNetwork randomTempoNetwork(int n, float tStart, float tEnd, float p1,
                                float p2, float p3);

/**
 * Functions for translating back and forth _E keys from pair<int, int> to
 * string. The stirng format is 3;4
 */
std::pair<int, int> strToPair(std::string key);

std::string pairToStr(std::pair<int, int> pair);

#endif // TEMPORALNETWORK_HPP