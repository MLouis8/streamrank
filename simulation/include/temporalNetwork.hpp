#ifndef TEMPORALNETWORK_HPP
#define TEMPORALNETWORK_HPP

#include "event.hpp"
#include "network.hpp"

#include <cmath>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

typedef vector<pair<float, float>> TimeItvs;
typedef pair<int, int> DTNode;
typedef unordered_map<int, vector<pair<int, int>>> FNeighbourhood;

class tempoNetwork {
public:
  tempoNetwork(Network aglo, vector<int> timeS);
  tempoNetwork(float tStart, float tEnd, int n, vector<TimeItvs> W,
               unordered_map<string, TimeItvs> E);
  float startTime() { return _tStart; }
  float endTime() { return _tEnd; }
  int size() { return _n; }
  TimeItvs getTimeItvs(int u) { return _W[u]; }
  TimeItvs getTimeItvs(int u, int v);
  float getEventVal(int eventId) { return _events[eventId].val(); }

  /**
   * Generate the tempoNetwork attributes from an aglomerated Network and a time
   * serie using the bijection between the two parameters and a stream graph and
   * using the bijection between the two parameters and a bipartite graph
   */
  void randomGeneration(Network aglo, vector<int> timeS);
  void genRdTimes();

  /**
   * Instantiate tempoNetwork class attributes linked to time: _events,
   * _nodeEvents, _edgeEvents necessary for accessing future neighbourhoods and
   * vanishing or appearance times. This function is automatically called when a
   * TempoNetwork is given from a file
   */
  void initTimeEvents();

  // Takes a time instant and returns the id of the event just before it
  int timeToEventId(float t);
  bool checkNodePres(int u, float t);
  bool checkEdgePres(int u, int v, float t);
  bool checkEdgeAtEvent(int u, int v, int event);
  float getNodeVanishT(int u, float t);
  float getNodeAppearT(int u, float t);
  int getNodeVanishEventId(int u, int k);
  int getNodeAppearEventId(int u, int k);
  /**
   * Returns the vector of node ids neighbours of u at the event time
   * corresponding to t
   */
  vector<int> getInstantNeighbours(int u, float t);
  /**
   * Returns the vector of node ids neighbours of u at the event time
   * corresponding to t
   */
  vector<int> getInstantEventNeighbours(int u, int eventId);
  /**
   * Returns a map linking a node v to its intervals for all nodes v linked to u
   * in the time segment [s, s_+(u)] such that [t_-(v), t_+(v)] is an interval
   * if t is in [s, s_+(u)]
   */
  FNeighbourhood getFutureNeighbours(int u, int idEvent);
  DTNode getRdLocation(DTNode prevLoc);

private:
  float _tStart;
  float _tEnd;
  int _n;
  vector<TimeItvs> _W;
  unordered_map<string, TimeItvs> _E;

  vector<Event> _events;
  vector<vector<int>> _nodeEvents;
  vector<vector<pair<int, int>>> _edgeEvents;
};

/**
 * Checks if the time given as parameter lies in one of the intervals
 */
bool timeValid(TimeItvs, float t);

#endif // TEMPORALNETWORK_HPP