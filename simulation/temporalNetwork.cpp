#include "include/temporalNetwork.hpp"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

TIntervals tempoNetwork::getTIntervals(int u, int v) {
  auto it = _E.find(pairToStr({u, v}));
  if (it != _E.end()) {
    return it->second;
  } else {
    return _E[pairToStr({v, u})];
  }
}

void tempoNetwork::initTimeEvents() {
  std::vector<Event> events = {Event(_tStart)};
  for (auto intervals : _W) {
    for (auto interval : intervals) {
      Event first = Event(interval.first);
      if (events.back() != first)
        events.push_back(first);
      events.push_back(Event(interval.second));
    }
  }
  for (const auto &pair : _E) {
    events.push_back(pair.second[0].first);
    for (auto interval : pair.second) {
      Event first = Event(interval.first);
      if (events.back() != first)
        events.push_back(first);
      events.push_back(Event(interval.second));
    }
  }
  std::sort(events.begin(), events.end(), eventComp);
  events.push_back(Event(_tEnd));
  events.erase(unique(events.begin(), events.end()), events.end());
  std::vector<std::vector<int>> nodeEvents;
  nodeEvents.assign(events.size(), {});
  for (int u = 0; u < _W.size(); u++) {
    int t = 0;
    for (auto interval : _W[u]) {
      while (events[t].getVal() < interval.first)
        t++;
      while (events[t + 1].getVal() <= interval.second) {
        nodeEvents[t].push_back(u);
        t++;
      }
    }
  }
  std::vector<std::vector<std::pair<int, int>>> edgeEvents;
  edgeEvents.assign(events.size(), {});
  for (auto &it : _E) {
    int t = 0;
    for (auto interval : it.second) {
      while (events[t] < interval.first)
        t++;
      while (events[t + 1] <= interval.second) {
        edgeEvents[t].push_back(strToPair(it.first));
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
  TIntervals intervals = getTIntervals(u);
  return timeValid(intervals, t);
}

bool tempoNetwork::checkEdgePres(int u, int v, float t) {
  TIntervals intervals = getTIntervals(u, v);
  return timeValid(intervals, t);
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

std::vector<std::tuple<int, int, int>>
tempoNetwork::getFutureNeighbours(int u, int idEvent) {
  if (not isTimeSet())
    throw missing_temporal_init(
        "must first initialise time events, see initTimeEvents().");
  int vanishId = getNodeVanishEventId(u, idEvent);
  std::vector<std::tuple<int, int, int>> res;
  for (int i = idEvent; i < vanishId; i++) {
    for (int neighbour : getInstantEventNeighbours(u, i)) {
      int prevSize = res.size();
      for (int j = 0; j < prevSize; j++) {
        if (std::get<0>(res[j]) == neighbour) {
          std::get<2>(res[j]) = i + 1;
        } else {
          res.push_back(std::make_tuple(neighbour, i, i + 1));
        }
      }
    }
  }
  return res;
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
  for (auto interval : _W[u]) {
    if (interval.first <= t && interval.second > t)
      return interval.second;
  }
  return t;
}

float tempoNetwork::getNodeAppearT(int u, float t) {
  for (auto interval : _W[u]) {
    if (interval.first <= t && interval.second > t)
      return interval.first;
  }
  return t;
}

bool timeValid(TIntervals intervals, float t) {
  for (auto interval : intervals) {
    if (interval.first <= t && interval.second >= t) {
      return true;
    }
  }
  return false;
}

TIntervals fillIntervals(float tEnd, float tStart, float p, std::mt19937 &gen,
                         std::normal_distribution<> &normalDis,
                         std::uniform_real_distribution<> &dis) {
  TIntervals res = {};
  int k = static_cast<int>(std::round(normalDis(gen)));
  float subIntervalSize = (tEnd - tStart) / k;
  std::pair<float, float> interval = {tStart, tStart};
  bool present = dis(gen) <= p;
  for (int i = 1; i < k; i++) {
    float t = tStart + i * subIntervalSize;
    interval.second = t;
    if (dis(gen) <= p) {
      if (not present) {
        res.push_back(interval);
        interval.first = t;
        present = true;
      }
    } else {
      if (present) {
        res.push_back(interval);
        interval.first = t;
        present = false;
      }
    }
  }
  return res;
}

tempoNetwork randomTempoNetwork(int n, float tStart, float tEnd, float p1,
                                float p2, float p3) {
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
  std::vector<TIntervals> W;
  // using p2 to determine the presence of a node
  // define a random generator following a gaussian law centered on tEnd -
  // tStart
  std::normal_distribution<> normalDis(tEnd - tStart, (tEnd - tStart) / 4.);
  for (int u = 0; u < n; u++) {
    TIntervals currentIntervals =
        fillIntervals(tEnd, tStart, p2, gen, normalDis, dis);
    W.push_back(currentIntervals);
  }
  std::unordered_map<std::string, TIntervals> E;
  // using p3 to determine the presence of an edge
  for (auto uv : edges) {
    TIntervals currentIntervals =
        fillIntervals(tEnd, tStart, p3, gen, normalDis, dis);
    E[pairToStr(uv)] = currentIntervals;
  }
  tempoNetwork net = tempoNetwork(tStart, tEnd, n, W, E);
  net.initTimeEvents();
  return net;
}

std::pair<int, int> strToPair(const std::string &key) {
  std::istringstream iss(key);
  std::pair<int, int> pair;
  std::string token;
  std::getline(iss, token, ';');
  pair.first = std::stoi(token);
  std::getline(iss, token, ';');
  pair.second = std::stoi(token);
  return pair;
}

std::string pairToStr(std::pair<int, int> pair) {
  return std::to_string(pair.first) + ";" + std::to_string(pair.second);
}