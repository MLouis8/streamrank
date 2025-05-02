#ifndef TEMPORALNETWORK_HPP
#define TEMPORALNETWORK_HPP

#include <cmath>
#include <map>
#include <string>
#include <utility>
#include <vector>

typedef std::vector<std::pair<float, float>> TIntervals;

/*
  Event times are just specific times when nodes or edges appear or disappear
  We define a specific class for this in order to redefine comparison operators:
  We would then like to know whether a node is present between two event times or not.
*/
class Event {
public:
  Event(float val): _val(val), _eps(1e-7) {}

  bool operator==(const Event& other) const {
    return std::fabs(_val - other._val) < _eps;
  }

  bool operator!=(const Event& other) const {
    return not (*this == other);
  }

  bool operator<(const Event& other) const {
    return _val < other._val;
  }

  bool operator<=(const Event& other) const {
    return _val < other._val || *this == other;
  }

  bool operator>(const Event& other) const {
    return not (*this <= other);
  }

  bool operator>=(const Event& other) const {
    return not (*this < other);
  }

private:
  float _val;
  float _eps;
};

class tempoNetwork {
public:
  tempoNetwork(float tStart, float tEnd, int n, std::vector<TIntervals> W,
               std::map<std::pair<int, int>, TIntervals> E)
      : _tStart(tStart), _tEnd(tEnd), _n(n), _W(W), _E(E),
        _timeAspectSet(false) {}
  float getStartingT() { return _tStart; }
  float getEndingT() { return _tEnd; }
  int getSize() { return _n; }
  TIntervals getTInterval(int u) { return _W[u]; }
  TIntervals getTInterval(int u, int v) { return _E[{u, v}]; }
  bool isTimeSet() { return _timeAspectSet; }

  // Necessary to initialize time aspect of a temporal network where data is accessible from a time value
  void initTimeEvents();
  Event timeToEvent(float t);
  bool checkNodePres(int u, float t);
  bool checkEdgePres(int u, int v, float t);
 

  std::vector<int> getInstantNeighbours(int u, float t);
  std::vector<int> getFutureNeighbours(int u, float t);
  float getNodeVanishT(int u, float t);
  float getNodeAppearT(int u, float t);
  float getEdgeVanishT(int u, int v, float t);
  float getEdgeAppearT(int u, int v, float t);

private:
  float _tStart;
  float _tEnd;
  int _n;
  std::vector<TIntervals> _W;
  std::map<std::pair<int, int>, TIntervals> _E;

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
tempoNetwork randomTempoNetwork(int n, float tStart, float tEnd, float p1, float p2, float p3);

#endif // TEMPORALNETWORK_HPP