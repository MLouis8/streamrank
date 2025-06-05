#ifndef TEMPORALNETWORK_HPP
#define TEMPORALNETWORK_HPP

#include "event.hpp"
#include "network.hpp"

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

typedef vector<pair<float, float>> TimeItvs;
typedef pair<int, int> DTNode;
typedef unordered_map<int, vector<int>> Fneighborhood;

class tempoNetwork {
public:
  tempoNetwork(Network aglo, vector<int> nodeTimeS, vector<int> edgeTimeS,
               float tStart, float tEnd);
  tempoNetwork(float tStart, float tEnd, int n, vector<TimeItvs> W,
               unordered_map<string, TimeItvs> E);
  tempoNetwork(float tStart, float tEnd, int n, vector<vector<int>> nodeEvents,
               vector<vector<pair<int, int>>> edgeEvents)
      : _tStart(tStart), _tEnd(tEnd), _n(n), _nodeEvents(nodeEvents),
        _edgeEvents(edgeEvents) {}
  float startTime() { return _tStart; }
  float endTime() { return _tEnd; }
  int size() { return _n; }
  TimeItvs getTimeItvs(int u) { return _W[u]; }
  TimeItvs getTimeItvs(int u, int v);
  float getEventVal(int e) { return _events[e].val(); }
  vector<vector<int>> &getNodeEvents() { return _nodeEvents; }
  vector<vector<pair<int, int>>> &getEdgeEvents() { return _edgeEvents; }
  int nbEvents() { return _nodeEvents.size(); }

  /**
   * Generate the tempoNetwork attributes from an aglomerated Network and a time
   * serie using the bijection between the two parameters and a stream graph and
   * using the bijection between the two parameters and a bipartite graph
   */
  void rdTNet(Network aglo, vector<int> nodeTimeS, vector<int> edgeTimeS);
  void genRdTimes(int nbEvents, float tStart, float tEnd);

  /**
   * Instantiate tempoNetwork class attributes linked to time: _events,
   * _nodeEvents, _edgeEvents necessary for accessing future neighborhoods and
   * vanishing or appearance times. This function is automatically called when a
   * TempoNetwork is given from a file
   */
  void initTimeEvents();

  // Takes a time instant and returns the id of the event just before it
  int timeToE(float t);
  bool checkNodePres(int u, float t);
  bool checkEdgePres(int u, int v, float t);
  bool checkEdgeAtEvent(int u, int v, int e);
  float nodeVanishT(int u, float t);
  float nodeAppearT(int u, float t);
  int nodeVanishE(int u, int k);
  int nodeAppearE(int u, int k);
  /**
   * Returns the vector of node ids of the neighbors of u at the event time
   * corresponding to time t
   */
  vector<int> instNeighbors(int u, float t);

  /**
   * Returns the vector of node ids neighbors of u at the event e
   */
  vector<int> instENeighbors(int u, int e);

  /**
   * Returns a map linking a node v to its intervals for all nodes v linked to u
   * in the time segment [s, s_+(u)] such that [t_-(v), t_+(v)] is an interval
   * if t is in [s, s_+(u)]
   */
  Fneighborhood directFutureNeighbors(int u, int e);

  /**
   * Returns a random node present at time t
   */
  int getRdLocation(int t);
  DTNode getRdTempoNode(DTNode prevLoc);

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