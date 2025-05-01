#include "include/temporalNetwork.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include <utility>
#include <vector>

void tempoNetwork::initTimeEvents() {
    std::vector<float> events;
    for (auto intervals: _W)
    {
        for (auto interval: intervals)
        {
            events.push_back(interval.first);
            events.push_back(interval.second);
        }
    }
    for (const auto& pair: _E)
    {
        events.push_back(pair.second[0].first);
        for (auto interval: pair.second)
        {
            events.push_back(interval.second);
        }
    }
    std::sort(events.begin(), events.end());
    std::vector<std::vector<int>> nodeEvents;
    nodeEvents.assign(events.size(), {});
    for (auto intervals: _W)
    {
        int i = 0;
        for (int j=0; j < events.size(); j++) {

        }
    }
    std::vector<std::vector<std::pair<int, int>>> edgeEvents;
    edgeEvents.assign(events.size(), {});
    for (const auto& pair: _E)
    {

    }
    _edgeEvents = edgeEvents;
    _nodeEvents = nodeEvents;
    _events = events;
  _timeAspectSet = true;
}

int tempoNetwork::timeToEvent(float t) {
  // TODO
}

bool tempoNetwork::checkNodePres(int u, float t) {
  TIntervals intervals = getTInterval(u);
  return timeValid(intervals, t);
}

bool tempoNetwork::checkEdgePres(int u, int v, float t) {
  TIntervals intervals = getTInterval(u, v);
  return timeValid(intervals, t);
}

std::vector<int> tempoNetwork::getInstantNeighbours(int u, float t) {
  if (not isTimeSet())
    throw missing_temporal_init(
        "must first initialise time events, see initTimeEvents().");
  int k = timeToEvent(t);
  return _nodeEvents[k];
}

std::vector<int> tempoNetwork::getFutureNeighbours(int u, float t) {
  // TODO
}

float tempoNetwork::getNodeVanishT(int u, float t) {
    // TODO
}

float tempoNetwork::getNodeAppearT(int u, float t) {
    // TODO
}

float tempoNetwork::getEdgeVanishT(int u, int v, float t) {
    // TODO
}

float tempoNetwork::getEdgeAppearT(int u, int v, float t) {
    // TODO
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
  std::map<std::pair<int, int>, TIntervals> E;
  // using p3 to determine the presence of an edge
  for (auto uv : edges) {
    TIntervals currentIntervals =
        fillIntervals(tEnd, tStart, p3, gen, normalDis, dis);
    E[uv] = currentIntervals;
  }
  tempoNetwork net = tempoNetwork(tStart, tEnd, n, W, E);
  net.initTimeEvents();
  return net;
}