#include "include/temporalNetwork.hpp"
#include "include/strHandler.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

TimeItvs tempoNetwork::getTimeItvs(int u, int v) {
  auto it = _E.find(pairToStr({u, v}));
  if (it != _E.end()) {
    return it->second;
  } else {
    return _E[pairToStr({v, u})];
  }
}

void tempoNetwork::initTimeEvents() {
  std::vector<Event> events = {Event(_tStart)};
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
  std::sort(events.begin(), events.end(), eventComp);
  events.push_back(Event(_tEnd));
  events.erase(unique(events.begin(), events.end()), events.end());
  std::vector<std::vector<int>> nodeEvents;
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
  std::vector<std::vector<std::pair<int, int>>> edgeEvents;
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
  _timeAspectSet = true;
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
  if (not isTimeSet())
    throw missing_temporal_init(
        "must first initialise time events, see initTimeEvents().");
  for (auto edge : _edgeEvents[event]) {
    if ((edge.first == u && edge.second == v) ||
        (edge.first == v && edge.second == u))
      return true;
  }
  return false;
}

int tempoNetwork::getNodeVanishEventId(int u, int k) {
  if (not isTimeSet())
    throw missing_temporal_init(
        "must first initialise time events, see initTimeEvents().");
  while (std::find(_nodeEvents[k].begin(), _nodeEvents[k].end(), u) !=
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

std::vector<int> tempoNetwork::getInstantEventNeighbours(int u, int eventId) {
  if (not isTimeSet())
    throw missing_temporal_init(
        "must first initialise time events, see initTimeEvents().");
  std::vector<int> instantNeighbours;
  for (auto edge : _edgeEvents[eventId]) {
    if (edge.first == u)
      instantNeighbours.push_back(edge.second);
    if (edge.second == u)
      instantNeighbours.push_back(edge.first);
  }
  return instantNeighbours;
}

std::vector<int> tempoNetwork::getInstantNeighbours(int u, float t) {
  return getInstantEventNeighbours(u, timeToEventId(t));
}

FNeighbourhood tempoNetwork::getFutureNeighbours(int u, int idEvent) {
  if (not isTimeSet())
    throw missing_temporal_init(
        "must first initialise time events, see initTimeEvents().");
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
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<int> w;
  for (auto nodeEvent : _nodeEvents) {
    w.push_back(nodeEvent.size());
  }
  std::discrete_distribution<> dis1(w.begin(), w.end());
  int s = dis1(gen);
  while (s == prevLoc.second)
    s = dis1(gen);
  std::uniform_int_distribution<> dis2(0, _nodeEvents[s].size() - 1);
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

TimeItvs fillItvs(float tEnd, float tStart, std::mt19937 &gen,
                  std::normal_distribution<> &normalDis,
                  std::uniform_real_distribution<> &dis) {
  int k = std::abs(static_cast<int>(std::round(normalDis(gen))));
  std::vector<float> tab;
  for (int i = 0; i < 2 * k; i++) {
    tab.push_back(tStart + dis(gen) * (tEnd - tStart));
  }
  std::sort(tab.begin(), tab.end());
  TimeItvs res;
  for (int j = 0; j < k; j++) {
    res.push_back({tab[j * 2], tab[j * 2 + 1]});
  }
  return res;
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

tempoNetwork randomTempoNetwork(int n, float tStart, float tEnd, float p1) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  std::vector<std::pair<int, int>> edges;
  // using p1 to determine if an edge is between two nodes
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (dis(gen) <= p1)
        edges.push_back({i, j});
    }
  }
  std::vector<TimeItvs> W;
  // using p2 to determine the presence of a node
  // define a random generator following a gaussian law centered on tEnd -
  // tStart
  std::normal_distribution<> normalDisNode(1, 1);
  for (int u = 0; u < n; u++) {
    TimeItvs currenTimeItvs = fillItvs(tEnd, tStart, gen, normalDisNode, dis);
    W.push_back(currenTimeItvs);
  }
  int u = 0;
  for (auto nodeItvs : W) {
    std::cout << "Node " << u << ": ";
    for (auto itv : nodeItvs) {
      std::cout << "[" << itv.first << ", " << itv.second << "], ";
    }
    std::cout << std::endl;
    u++;
  }
  std::unordered_map<std::string, TimeItvs> E;
  std::normal_distribution<> normalDisEdge(1, 0.1);
  // using p3 to determine the presence of an edge
  for (auto uv : edges) {
    TimeItvs nodeInter = itvsInter(W[uv.first], W[uv.second]);
    TimeItvs edgeItvs;
    for (auto interItv : nodeInter) {
      TimeItvs subEdgeItvs =
          fillItvs(interItv.second, interItv.first, gen, normalDisEdge, dis);
      for (auto edgeItv : subEdgeItvs)
        edgeItvs.push_back(edgeItv);
    }
    E[pairToStr(uv)] = edgeItvs;
  }
  tempoNetwork net = tempoNetwork(tStart, tEnd, n, W, E);

  for (auto &[key, value] : E) {
    std::cout << "Edge " << key << ": ";
    for (auto itv : value) {
      std::cout << "[" << itv.first << ", " << itv.second << "], ";
    }
    std::cout << std::endl;
  }
  net.initTimeEvents();
  return net;
}