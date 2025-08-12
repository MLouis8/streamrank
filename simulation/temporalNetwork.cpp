#include "include/temporalNetwork.hpp"
#include "include/event.hpp"
#include "include/network.hpp"
#include "include/rdLib.hpp"
#include "include/strHandler.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

tempoNetwork::tempoNetwork(int n, int sumNodes, int nbEvents, float p,
                           float tStart, float tEnd, bool atomicEdges) {
  vector<int> eventPerNodeS, nodePerEventS;
  // do {
  eventPerNodeS = rdTimeSeries(sumNodes, n, nbEvents);
  nodePerEventS = rdTimeSeries(sumNodes, nbEvents, n);
  cout << "\ntrying a randomly generated pair of sequences...";
  // } while (notCompatible(eventPerNodeS, nodePerEventS));

  Bipartite tNodes = rdBipartiteFromDegrees(eventPerNodeS, nodePerEventS);
  _n = eventPerNodeS.size();
  _nodeEvents = vector<vector<int>>(nbEvents);
  for (auto e : tNodes) {
    _nodeEvents[e.second].push_back(e.first);
  }
  _edgeEvents = vector<vector<pair<int, int>>>(nbEvents);
  if (atomicEdges) {
    for (int i = 0; i < nbEvents; i++) {
      Network currentNet(_nodeEvents[i].size(), p);
      vector<pair<int, int>> rawEdges = currentNet.getEdges();
      for (auto e : rawEdges)
        _edgeEvents[i].push_back(
            {_nodeEvents[i][e.first], _nodeEvents[i][e.second]});
    }
  } else {
    vector<Chunks> nodeChunks;
    nodeChunks.push_back(getChunks(0));
    for (int u = 0; u < _n; u++) {
      for (int v = u + 1; v < _n; v++) {
        if (nodeChunks.size() <= v) {
          nodeChunks.push_back(getChunks(v));
        }
        int uChunkStart = 0, uChunkEnd = 0;
        int vChunkStart = 0, vChunkEnd = 0;
        while (uChunkStart < nbEvents || vChunkStart < nbEvents) {
          uChunkEnd = nodeVanishE(u, uChunkStart);
          while (uChunkEnd == uChunkStart) {
            uChunkStart += 1;
            uChunkEnd = nodeVanishE(u, uChunkStart);
          }
          while (vChunkStart <= uChunkEnd) {
          }

          vChunkEnd = nodeVanishE(v, vChunkStart);
          while (vChunkEnd == vChunkStart) {
            vChunkStart += 1;
            vChunkEnd = nodeVanishE(v, vChunkStart);
          }
          while (uChunkStart > vChunkEnd) {
          }
          while (vChunkStart > uChunkEnd) {
          }
        }
      }
    }

    // for (int node = 0; node < _n; node++) {
    //   int startChunkE = 0;
    //   while (startChunkE < nbEvents) {
    //     int endChunk = nodeVanishE(node, startChunkE);
    //     Fneighborhood pNeighbours;
    //     // int currentChunkSize = nodeVanishE(node, chunkId) - chunkId + 1;
    //     for (int i = startChunkE; i <= endChunk; i++) {
    //       for (int otherNode : _nodeEvents[i]) {
    //         if (otherNode != node) {
    //           if (find(pNeighbours.begin(), pNeighbours.end(), otherNode) !=
    //               pNeighbours.end()) {
    //             pNeighbours[otherNode].push_back();
    //           } else {
    //             pNeighbours[otherNode] = ;
    //           }
    //         }
    //       }
    //     }
    //   }
    // }
  }

  _tStart = tStart;
  _tEnd = tEnd;
  vector<Event> events;
  float step = (_tEnd - _tStart) / nbEvents;
  for (int e = 0; e < nbEvents; e++) {
    events.push_back(Event(_tStart + step * e));
  }
  _events = events;
}

tempoNetwork::tempoNetwork(Network aglo, vector<int> nodeTimeS,
                           vector<int> edgeTimeS, float tStart, float tEnd) {
  assert(nodeTimeS.size() == edgeTimeS.size());
  rdTNet(aglo, nodeTimeS, edgeTimeS);
  genRdTimes(nodeTimeS.size(), tStart, tEnd);
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

tempoNetwork::tempoNetwork(float tStart, float tEnd, int n,
                           vector<vector<int>> nodeEvents,
                           vector<vector<pair<int, int>>> edgeEvents) {
  _tStart = tStart;
  _tEnd = tEnd;
  _n = n;
  _nodeEvents = nodeEvents;
  _edgeEvents = edgeEvents;
  int nbEvents = _nodeEvents.size();
  assert(nbEvents == _edgeEvents.size());
  vector<Event> events;
  float step = (_tEnd - _tStart) / nbEvents;
  for (int e = 0; e < nbEvents; e++) {
    events.push_back(Event(_tStart + step * e));
  }
  _events = events;
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

int tempoNetwork::timeToE(float t) {
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

int tempoNetwork::nodeVanishE(int u, int k) {
  do {
    k++;
  } while (k < _nodeEvents.size() &&
           find(_nodeEvents[k].begin(), _nodeEvents[k].end(), u) !=
               _nodeEvents[k].end());
  return k - 1;
}

float tempoNetwork::nodeVanishT(int u, float t) {
  for (auto itv : _W[u]) {
    if (itv.first <= t && itv.second > t)
      return itv.second;
  }
  return t;
}

float tempoNetwork::nodeAppearT(int u, float t) {
  for (auto itv : _W[u]) {
    if (itv.first <= t && itv.second > t)
      return itv.first;
  }
  return t;
}

vector<int> tempoNetwork::instENeighbors(int u, int e) {
  vector<int> instNeighbors;
  for (auto edge : _edgeEvents[e]) {
    if (edge.first == u)
      instNeighbors.push_back(edge.second);
    if (edge.second == u)
      instNeighbors.push_back(edge.first);
  }
  return instNeighbors;
}

vector<int> tempoNetwork::instNeighbors(int u, float t) {
  return instENeighbors(u, timeToE(t));
}

Fneighborhood tempoNetwork::directFutureNeighbors(int u, int e) {
  int vanishE = nodeVanishE(u, e);
  Fneighborhood res;
  for (int i = e; i <= vanishE; i++) {
    vector<int> ineigh = instENeighbors(u, i);
    for (int neighbor : ineigh) {
      if (res.find(neighbor) == res.end()) {
        res[neighbor] = {i};
      } else {
        res[neighbor].push_back(i);
      }
    }
  }
  return res;
}

Fneighborhood tempoNetwork::futureNeighbours(int u, int e) {
  int vanishE = nodeVanishE(u, e);
  Fneighborhood res;
  vector<int> ineigh;
  int i = e;
  for (; i <= vanishE; i++) {
    ineigh = instENeighbors(u, i);
    for (int neighbor : ineigh) {
      if (res.find(neighbor) == res.end()) {
        res[neighbor] = {i};
      } else {
        res[neighbor].push_back(i);
      }
    }
  }
  for (int lastneigh : ineigh) {
    int vanishE = nodeVanishE(lastneigh, i);
    for (int j = i; j < vanishE; j++)
      res[lastneigh].push_back(j);
  }
  return res;
}

DTNode tempoNetwork::getRdTempoNode(int u, int k) {
  random_device rd;
  mt19937 gen(rd());
  vector<int> w;
  for (int e = k; e <= nodeVanishE(u, k); e++) {
    w.push_back(_nodeEvents[e].size());
  }
  discrete_distribution<> dis1(w.begin(), w.end());
  int t = dis1(gen);
  uniform_int_distribution<> dis2(0, _nodeEvents[t].size() - 1);
  int v = dis2(gen);
  return {v, t};
}

int tempoNetwork::getRdLocation(int t) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dis(0, _nodeEvents[t].size() - 1);
  return _nodeEvents[t][dis(gen)];
}

bool timeValid(TimeItvs itvs, float t) {
  for (auto itv : itvs) {
    if (itv.first <= t && itv.second >= t)
      return true;
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
  vector<vector<int>> nodeEvents(nodeTimeS.size());
  for (auto e : tNodes) {
    nodeEvents[e.second].push_back(e.first);
  }
  _nodeEvents = nodeEvents;
  vector<vector<pair<int, int>>> edgeEvents(edgeTimeS.size());
  for (auto e : tEdges) {
    pair<int, int> edge = aglo.getEdge(e.first);
    edgeEvents[e.second].push_back(edge);
  }
  _edgeEvents = edgeEvents;
}

void tempoNetwork::genRdTimes(int nbEvents, float tStart, float tEnd) {
  _tStart = tStart;
  _tEnd = tEnd;
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(_tStart, _tEnd);
  vector<float> times;
  for (int i = 0; i < nbEvents; i++) {
    times.push_back(dis(gen));
  }
  sort(times.begin(), times.end());
  vector<Event> events;
  for (auto time : times)
    events.push_back(Event(time));
  _events = events;
}

float tempoNetwork::avgChunkSize() {
  float nbChunks = 0;
  float sumChunks = 0;
  for (int node = 0; node < _n; node++) {
    Chunks nodeChunks = getChunks(node);
    for (auto chunk : nodeChunks) {
      nbChunks += 1;
      sumChunks += chunk.second - chunk.first + 1;
    }
  }
  return sumChunks / nbChunks;
}

Chunks tempoNetwork::getChunks(int node) {
  vector<pair<int, int>> res;
  int start = 0;
  while (start < _nodeEvents.size()) {
    while (start < _nodeEvents.size() &&
           find(_nodeEvents[start].begin(), _nodeEvents[start].end(), node) ==
               _nodeEvents[start].end())
      start += 1;
    if (start >= _nodeEvents.size())
      return res;
    int end = nodeVanishE(node, start);
    res.push_back({start, end});
    start = end + 1;
  }
  return res;
}