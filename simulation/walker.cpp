#include "include/walker.hpp"
#include "include/integral.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

template <> int Walker<int>::step(Network &net, float alpha) {
  random_device rd;
  default_random_engine eng(rd());
  uniform_real_distribution<float> jumpDis(0, 1);

  vector<int> neigh = net.neighbors(_pos);
  if (jumpDis(eng) > alpha || neigh.size() == 0) { // jump anywhere in the graph
    _pos = net.getRdLocation(_pos);
  } else { // walk to a neighbor
    vector<float> weights = net.neighborsWeights(_pos);
    discrete_distribution<> neighborDis(weights.begin(), weights.end());
    _pos = neighborDis(eng);
  }
  return _pos;
}

template <>
DTNode Walker<DTNode>::approxStep(tempoNetwork &tnet, float alpha,
                                  function<float(float)> h) {
  random_device rd;
  default_random_engine eng(rd());
  uniform_real_distribution<float> jumpDis(0, 1);

  Fneighborhood dfnhb = tnet.directFutureNeighbors(_pos.first, _pos.second);

  if (jumpDis(eng) > alpha || dfnhb.size() == 0) {
    _pos = tnet.getRdTempoNode(_pos.first, _pos.second + 1);
  } else {
    // choose v amongts the future neighbors of u
    vector<float> w1;
    vector<int> neighbors;
    for (auto &[key, val] : dfnhb) {
      w1.push_back(val.size());
      neighbors.push_back(key);
    }
    discrete_distribution<> dis1(w1.begin(), w1.end());
    int v = neighbors[dis1(eng)];

    // select an event
    int uEnd = tnet.nodeVanishE(_pos.first, _pos.second);
    int vStart = dfnhb[v][0];
    int vEnd = dfnhb[v].back(); // be careful maybe v lifespan have gaps
                                // between uStart and uEnd
    vector<int> possibleEvents;
    vector<float> w2;
    float tk1 = tnet.getEventVal(_pos.second + 1);
    int l = _pos.second + 1;
    while (l <= vEnd && l < tnet.nbEvents() - 1) {
      possibleEvents.push_back(l);
      float tl = tnet.getEventVal(l);
      float w = 0;
      for (int i = _pos.second; i < l; i++) {
        if (i <= uEnd && l <= tnet.nodeVanishE(v, i) &&
            tnet.checkEdgeAtEvent(_pos.first, v, i) &&
            tnet.checkEdgeAtEvent(_pos.first, v, i + 1)) {
          float div = 0;
          for (int j = i; j < tnet.nodeVanishE(v, l); j++) {
            float tj = tnet.getEventVal(j);
            float tj1 = tnet.getEventVal(j + 1);
            div += (tj1 - tj) * h(tj - tk1);
          }
          float ti = tnet.getEventVal(i);
          float ti1 = tnet.getEventVal(i + 1);
          w += (ti1 - ti) / div;
        }
      }
      w = w * h(tl - tk1);
      w2.push_back(w);
      l++;
    }
    discrete_distribution<> dis2(w2.begin(), w2.end());
    int t = possibleEvents[dis2(eng)];
    _pos = {v, t};
  }
  return _pos;
}

template <>
DTNode Walker<DTNode>::upperBound(tempoNetwork &tnet, float alpha,
                                  function<float(float)> h) {
  random_device rd;
  default_random_engine eng(rd());
  Fneighborhood dfnhb = tnet.directFutureNeighbors(_pos.first, _pos.second);
  Fneighborhood fnhb = tnet.futureNeighbours(_pos.first, _pos.second);
  uniform_real_distribution<float> jumpDis(0, 1);
  if (jumpDis(eng) > alpha || dfnhb.size() == 0) {
    _pos = tnet.getRdTempoNode(_pos.first, _pos.second);
  } else {
    // choose v amongts the future neighbors of u
    vector<float> w1;
    vector<int> neighbors;
    // cout << "\nNeighbours of " << _pos.first << ' ' << _pos.second;
    for (auto &[key, val] : fnhb) {
      // cout << "\n" << key << ':';
      // for (auto v : val) {
      //   cout << ' ' << v;
      // }
      w1.push_back(val.size());
      neighbors.push_back(key);
    }
    discrete_distribution<> dis1(w1.begin(), w1.end());
    int v = neighbors[dis1(eng)];
    // select an event
    int uEnd = tnet.nodeVanishE(_pos.first, _pos.second);
    vector<int> possibleEvents;
    vector<float> w2;
    float tk = tnet.getEventVal(_pos.second);
    float tk1 = tnet.getEventVal(_pos.second + 1);

    for (auto l : fnhb[v]) {
      // cout << "\nPossible event is " << l;
      possibleEvents.push_back(l);
      float tl = tnet.getEventVal(l);
      float w = 0.;
      for (int i = _pos.second; i <= l; i++) {
        int iEnd = tnet.nodeVanishE(v, i);
        if (i <= uEnd && l <= iEnd && tnet.checkEdgeAtEvent(_pos.first, v, i)) {
          int i1End = tnet.nodeVanishE(v, i + 1);
          float ti1 = tnet.getEventVal(i + 1);
          float ti = tnet.getEventVal(i);
          float ti1End = tnet.getEventVal(i1End);
          if (ti1 == ti1End) {
            ti1End += 1;
            if (i1End + 1 >= tnet.nbEvents())
              ti1End += 1;
            else
              ti1End = tnet.getEventVal(i1End + 1);
          }
          float div = integral(ti1 - tk, ti1End - tk, h, 100);
          w += (ti1 - ti) / div;
        }
      }
      w = w * h(tl - tk1);
      // if (w <= 0.00001) {
      //   cout << w << endl
      //        << _pos.first << ' ' << _pos.second << endl
      //        << v << ' ' << l << endl;
      // }
      w2.push_back(w);
      l++;
    }
    int t;
    if (accumulate(w2.begin(), w2.end(), 0.) > 0) {
      discrete_distribution<> dis2(w2.begin(), w2.end());
      t = possibleEvents[dis2(eng)];
    } else {
      uniform_int_distribution<> dis3(0, possibleEvents.size() - 1);
      t = possibleEvents[dis3(eng)];
    }
    _pos = {v, t};
  }
  // cout << "\nNext node is " << _pos.first << ' ' << _pos.second;
  return _pos;
}

template <>
DTNode Walker<DTNode>::lowerBound(tempoNetwork &tnet, float alpha,
                                  function<float(float)> h) {
  random_device rd;
  default_random_engine eng(rd());
  uniform_real_distribution<float> jumpDis(0, 1);
  Fneighborhood dfnhb = tnet.directFutureNeighbors(_pos.first, _pos.second);

  if (jumpDis(eng) > alpha || dfnhb.size() == 0) {
    _pos = tnet.getRdTempoNode(_pos.first, _pos.second);
  } else {
    // choose v amongts the future neighbors of u
    vector<float> w1;
    vector<int> neighbors;
    for (auto &[key, val] : dfnhb) {
      w1.push_back(val.size());
      neighbors.push_back(key);
    }
    discrete_distribution<> dis1(w1.begin(), w1.end());
    int v = neighbors[dis1(eng)];

    // select an event
    int uEnd = tnet.nodeVanishE(_pos.first, _pos.second);
    int vStart = dfnhb[v][0];
    int vEnd = dfnhb[v].back();
    vector<int> possibleEvents;
    vector<float> w2;
    float tk1 = tnet.getEventVal(_pos.second + 1);
    for (int l = _pos.second; l <= vEnd && l < tnet.nbEvents() - 1; l++) {
      possibleEvents.push_back(l);
      float tl = tnet.getEventVal(l);
      float w = 1;
      for (int i = _pos.second + 1; i < l; i++) {
        int iEnd = tnet.nodeVanishE(v, i);
        if (i <= uEnd && l <= iEnd && tnet.checkEdgeAtEvent(_pos.first, v, i)) {
          float ti1 = tnet.getEventVal(i + 1);
          float ti = tnet.getEventVal(i);
          float tiEnd = tnet.getEventVal(ti);
          float div = integral(ti - tk1, tiEnd - tk1, h, 100);
          w += (ti1 - ti) / div;
        }
      }
      float tl1 = tnet.getEventVal(l + 1);
      float tk = tnet.getEventVal(_pos.second);
      w = w * h(tl1 - tk);
      w2.push_back(w);
    }
    discrete_distribution<> dis2(w2.begin(), w2.end());
    int t = possibleEvents[dis2(eng)];
    _pos = {v, t};
  }
  return _pos;
}