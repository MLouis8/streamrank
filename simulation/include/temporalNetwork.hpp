#ifndef TEMPORALNETWORK_HPP
#define TEMPORALNETWORK_HPP

#include "event.hpp"
#include <cmath>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

typedef std::vector<std::pair<float, float>> TIntervals;

class tempoNetwork {
public:
  tempoNetwork(float tStart, float tEnd, int n, std::vector<TIntervals> W,
               std::unordered_map<std::pair<int, int>, TIntervals> E)
      : _tStart(tStart), _tEnd(tEnd), _n(n), _W(W), _E(E),
        _timeAspectSet(false) {}
  float getStartingT() { return _tStart; }
  float getEndingT() { return _tEnd; }
  int getSize() { return _n; }
  TIntervals getTIntervals(int u) { return _W[u]; }
  TIntervals getTIntervals(int u, int v);
  bool isTimeSet() { return _timeAspectSet; }

  // Necessary to initialize time aspect of a temporal network where data is
  // accessible from a time value
  void initTimeEvents();
  int timeToEventId(float t);
  bool checkNodePres(int u, float t);
  bool checkEdgePres(int u, int v, float t);
  float getNodeVanishT(int u, float t);
  float getNodeAppearT(int u, float t);

  std::vector<int> getInstantNeighbours(int u, float t);
  std::vector<int> getFutureNeighbours(int u, float t);
  int getNodeVanishEventId(int u, int k);
  int getNodeAppearEventId(int u, int k);

private:
  float _tStart;
  float _tEnd;
  int _n;
  std::vector<TIntervals> _W;
  std::unordered_map<std::pair<int, int>, TIntervals> _E;

  bool _timeAspectSet;
  std::vector<Event> _events;
  std::vector<std::vector<int>> _nodeEvents;
  std::vector<std::vector<std::pair<int, int>>> _edgeEvents;
};

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

#endif // TEMPORALNETWORK_HPP