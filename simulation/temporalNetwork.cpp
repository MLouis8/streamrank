#include "include/temporalNetwork.hpp"
#include "include/bipartite.hpp"
#include "include/network.hpp"
#include "include/strHandler.hpp"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

tempoNetwork::tempoNetwork(Network aglo, vector<int> nodeTimeS,
                           vector<int> edgeTimeS) {
  rdTNet(aglo, nodeTimeS, edgeTimeS);
  genRdTimes();
}

tempoNetwork::tempoNetwork(float tStart, float tEnd, int n, vector<TimeItvs> W,
                           unordered_map<string, TimeItvs> E) {
  _tStart = tStart;
  _tEnd = tEnd;
  _n = n;
  _W = W;
  _E = E;
  initTimeEvents();
}

TimeItvs tempoNetwork::getTimeItvs(int u, int v) {
  auto it = _E.find(pairToStr({u, v}));
  if (it != _E.end()) {
    return it->second;
  } else {
    return _E[pairToStr({v, u})];
  }
}

void tempoNetwork::initTimeEvents() {
  vector<Event> events = {Event(_tStart)};
  for (auto itvs : _W) {
    for (auto itv : itvs) {
      events.push_back(Event(itv.first));
      events.push_back(Event(itv.second));
    }
  }
  for (auto &[key, val] : _E) {
    for (auto itv : val) {
      events.push_back(Event(itv.first));
      events.push_back(Event(itv.second));
    }
  }
  sort(events.begin(), events.end(), eventComp);
  events.push_back(Event(_tEnd));
  events.erase(unique(events.begin(), events.end()), events.end());
  vector<vector<int>> nodeEvents;
  nodeEvents.assign(events.size(), {});
  for (int u = 0; u < _W.size(); u++) {
    int t = 0;
    for (auto itv : _W[u]) {
      while (events[t].val() < itv.first)
        t++;
      while (events[t + 1].val() <= itv.second) {
        nodeEvents[t].push_back(u);
        t++;
      }
    }
  }
  vector<vector<pair<int, int>>> edgeEvents;
  edgeEvents.assign(events.size(), {});
  for (auto &[key, val] : _E) {
    int t = 0;
    for (auto itv : val) {
      while (events[t] < itv.first)
        t++;
      while (events[t + 1] <= itv.second) {
        edgeEvents[t].push_back(strToPair(key));
        t++;
      }
    }
  }
  _edgeEvents = edgeEvents;
  _nodeEvents = nodeEvents;
  _events = events;
}

int tempoNetwork::timeToEventId(float t) {
  Event tEvent = Event(t);
  int i = 0;
  while (_events[i] <= tEvent) {
    i++;
  }
  return i - 1;
}

bool tempoNetwork::checkNodePres(int u, float t) {
  TimeItvs itvs = getTimeItvs(u);
  return timeValid(itvs, t);
}

bool tempoNetwork::checkEdgePres(int u, int v, float t) {
  TimeItvs itvs = getTimeItvs(u, v);
  return timeValid(itvs, t);
}

bool tempoNetwork::checkEdgeAtEvent(int u, int v, int event) {
  for (auto edge : _edgeEvents[event]) {
    if ((edge.first == u && edge.second == v) ||
        (edge.first == v && edge.second == u))
      return true;
  }
  return false;
}

int tempoNetwork::getNodeVanishEventId(int u, int k) {
  while (find(_nodeEvents[k].begin(), _nodeEvents[k].end(), u) !=
         _nodeEvents[k].end())
    k++;
  return k;
}

float tempoNetwork::getNodeVanishT(int u, float t) {
  for (auto itv : _W[u]) {
    if (itv.first <= t && itv.second > t)
      return itv.second;
  }
  return t;
}

float tempoNetwork::getNodeAppearT(int u, float t) {
  for (auto itv : _W[u]) {
    if (itv.first <= t && itv.second > t)
      return itv.first;
  }
  return t;
}

vector<int> tempoNetwork::getInstantEventNeighbours(int u, int eventId) {
  vector<int> instantNeighbours;
  for (auto edge : _edgeEvents[eventId]) {
    if (edge.first == u)
      instantNeighbours.push_back(edge.second);
    if (edge.second == u)
      instantNeighbours.push_back(edge.first);
  }
  return instantNeighbours;
}

vector<int> tempoNetwork::getInstantNeighbours(int u, float t) {
  return getInstantEventNeighbours(u, timeToEventId(t));
}

FNeighbourhood tempoNetwork::getFutureNeighbours(int u, int idEvent) {
  int vanishId = getNodeVanishEventId(u, idEvent);
  FNeighbourhood res;
  for (int i = idEvent; i < vanishId; i++) {
    for (int neighbour : getInstantEventNeighbours(u, i)) {
      if (res.find(neighbour) == res.end()) {
        res[neighbour] = {{i, i + 1}};
      } else {
        if (res[neighbour].back().second == i) {
          res[neighbour].back().second = i + 1;
        } else {
          res[neighbour].push_back({i, i + 1});
        }
      }
    }
  }
  return res;
}

DTNode tempoNetwork::getRdLocation(DTNode prevLoc) {
  random_device rd;
  mt19937 gen(rd());
  vector<int> w;
  for (auto nodeEvent : _nodeEvents) {
    w.push_back(nodeEvent.size());
  }
  discrete_distribution<> dis1(w.begin(), w.end());
  int s = dis1(gen);
  while (s == prevLoc.second)
    s = dis1(gen);
  uniform_int_distribution<> dis2(0, _nodeEvents[s].size() - 1);
  int u = dis2(gen);
  while (u == prevLoc.first)
    u = dis2(gen);
  return {u, s};
}

bool timeValid(TimeItvs itvs, float t) {
  for (auto itv : itvs) {
    if (itv.first <= t && itv.second >= t) {
      return true;
    }
  }
  return false;
}

TimeItvs itvsInter(TimeItvs itvU, TimeItvs itvV) {
  int idU = 0;
  int idV = 0;
  TimeItvs inter;
  while (idU < itvU.size() && idV < itvV.size()) {
    if (itvU[idU].first <= itvV[idV].first) {
      if (itvU[idU].second <= itvV[idV].first) {
        idU++;
      } else {
        if (itvU[idU].second <= itvV[idV].second) {
          inter.push_back({itvV[idV].first, itvU[idU].second});
          idU++;
        } else {
          inter.push_back({itvV[idV].first, itvV[idV].second});
          idV++;
        }
      }
    } else {
      if (itvV[idV].second <= itvU[idU].first) {
        idV++;
      } else {
        if (itvV[idV].second <= itvU[idU].second) {
          inter.push_back({itvU[idU].first, itvV[idV].second});
          idV++;
        } else {
          inter.push_back({itvU[idU].first, itvU[idU].second});
          idU++;
        }
      }
    }
  }
  return inter;
}

void tempoNetwork::rdTNet(Network aglo, vector<int> nodeTimeS,
                          vector<int> edgeTimeS) {
  // Generate nodes and edges events
  vector<int> nodeAglo = aglo.nodeWeight();
  vector<int> edgeAglo = aglo.adjacencyWeight();
  int sumDegreesN = accumulate(nodeAglo.begin(), nodeAglo.end(), 0);
  int sumDegreesE = accumulate(edgeAglo.begin(), edgeAglo.end(), 0);
  if (accumulate(nodeTimeS.begin(), nodeTimeS.end(), 0) != sumDegreesN)
    throw invalid_argument("the sum of the aglomerated graph node weights must "
                           "be the same to the sum of the node time serie.");
  if (accumulate(edgeTimeS.begin(), edgeTimeS.end(), 0) != sumDegreesE)
    throw invalid_argument("the sum of the aglomerated graph edge weights must "
                           "be the same to the sum of the edge time serie.");
  Bipartite tNodes = rdBipartiteFromDegrees(nodeAglo, nodeTimeS);
  Bipartite tEdges = rdBipartiteFromDegrees(edgeAglo, edgeTimeS);
  _n = nodeTimeS.size();
  vector<vector<int>> nodeEvents(nodeTimeS.size(), {-1});
  for (auto e : tNodes) {
    if (nodeEvents[e.second][0] == -1)
      nodeEvents[e.second][0] = e.first;
    else
      nodeEvents[e.second].push_back(e.first);
  }
  _nodeEvents = nodeEvents;
  vector<vector<pair<int, int>>> edgeEvents(edgeTimeS.size(), {{-1, -1}});
  for (auto e : tEdges) {
    pair<int, int> edge = aglo.getEdge(e.first);
    if (edgeEvents[e.second][0].first == -1)
      edgeEvents[e.second][0] = edge;
    else
      edgeEvents[e.second].push_back(edge);
  }
  _edgeEvents = edgeEvents;
}