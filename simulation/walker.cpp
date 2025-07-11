#include "include/walker.hpp"
#include "include/integral.hpp"
#include <iostream>
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

// template <>
// DTNode Walker<DTNode>::step(tempoNetwork &tnet, float alpha,
//                             function<float(float)> h) {
//   random_device rd;
//   default_random_engine eng(rd());
//   Fneighborhood fnhb = tnet.getFutureneighbors(_pos.first, _pos.second);

//   uniform_real_distribution<float> jumpDis(0, 1);
//   if (jumpDis(eng) > alpha || fnhb.size() == 0) {
//     _pos = {tnet.getRdLocation(_pos.second), _pos.second + 1};
//   } else {
//   }
//   return _pos;
// }

template <>
DTNode Walker<DTNode>::approxStep(tempoNetwork &tnet, float alpha,
                                  function<float(float)> h) {
  random_device rd;
  default_random_engine eng(rd());
  uniform_real_distribution<float> jumpDis(0, 1);

  Fneighborhood dfnhb = tnet.directFutureNeighbors(_pos.first, _pos.second);

  if (jumpDis(eng) > alpha || dfnhb.size() == 0) {
    _pos = {tnet.getRdLocation(_pos.second + 1), _pos.second + 1};
    cout << "\njump: v:" << _pos.first << " t: " << _pos.second;
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
    // int uEnd = tnet.nodeVanishE(_pos.first, _pos.second);
    // int vStart = dfnhb[v][0];
    // int vEnd = dfnhb[v][0];
    // for (auto e : dfnhb[v]) {
    //   if (vEnd >= _pos.second)
    //     break;
    //   if (e - vEnd > 1)
    //     vStart = e;
    //   vEnd = e;
    // }
    // vector<int> possibleEvents;
    // vector<float> w2;
    // float tk1 = tnet.getEventVal(_pos.second + 1);
    // for (int l = _pos.second + 1; l <= vEnd; l++) {
    //   possibleEvents.push_back(l);
    //   float tl = tnet.getEventVal(l);
    //   float w = 0.;
    //   for (int i = _pos.second; i < l; i++) {
    //     if (i <= uEnd && vStart <= i &&
    //         tnet.checkEdgeAtEvent(_pos.first, v, i) &&
    //         tnet.checkEdgeAtEvent(_pos.first, v, i + 1)) {
    //       float div = 0;
    //       for (int j = i + 1;; j++) {
    //         float tj1 = tnet.getEventVal(j + 1);
    //         float tj = tnet.getEventVal(j);
    //         div += (tj1 - tj) * h(tj - tk1);
    //       }
    //       float ti1 = tnet.getEventVal(i + 1);
    //       float ti = tnet.getEventVal(i);
    //       w += (ti1 - ti) / div;
    //     }
    //   }
    //   w = w * h(tl - tk1);
    //   w2.push_back(w);
    // }
    // discrete_distribution<> dis2(w2.begin(), w2.end());
    // int t = possibleEvents[dis2(eng)];
    cout << "\nnormal: v: " << v << ", t " << _pos.second + 1;
    _pos = {v, _pos.second + 1}; // t};
  }
  return _pos;
}

template <>
DTNode Walker<DTNode>::upperBound(tempoNetwork &tnet, float alpha,
                                  function<float(float)> h) {
  random_device rd;
  default_random_engine eng(rd());
  Fneighborhood dfnhb = tnet.directFutureNeighbors(_pos.first, _pos.second);

  uniform_real_distribution<float> jumpDis(0, 1);
  if (jumpDis(eng) > alpha || dfnhb.size() == 0) {
    _pos = {tnet.getRdLocation(_pos.second + 1), _pos.second + 1};
  } else {
    // choose v amongts the future neighbors of u
    // here we weight each neihbor with the total size, shouldn't we weight
    // with the actual interval size ?
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
    int vEnd = dfnhb[v][0];
    for (auto e : dfnhb[v]) {
      if (vEnd >= _pos.second)
        break;
      if (e - vEnd > 1)
        vStart = e;
      vEnd = e;
    }
    vector<int> possibleEvents;
    vector<float> w2;
    float tk1 = tnet.getEventVal(_pos.second + 1);
    float tk = tnet.getEventVal(_pos.second);
    for (int l = _pos.second + 1; l <= vEnd; l++) {
      possibleEvents.push_back(l);
      float tl = tnet.getEventVal(l);
      float w = 0.;
      for (int i = _pos.second; i <= l; i++) {
        if (i <= uEnd && vStart <= i &&
            tnet.checkEdgeAtEvent(_pos.first, v, i) &&
            tnet.checkEdgeAtEvent(_pos.first, v, i + 1)) {
          float ti1 = tnet.getEventVal(i + 1);
          float ti = tnet.getEventVal(i);
          float ti1End = tnet.nodeVanishT(v, ti1);
          float div = integral(ti1 - tk, ti1End - tk, h, 100);
          w += (ti1 - ti) / div;
        }
      }
      w = w * h(tl - tk1);
      w2.push_back(w);
    }
    discrete_distribution<> dis2(w2.begin(), w2.end());
    int t = possibleEvents[dis2(eng)];
    _pos = {v, t};
  }
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
    _pos = {tnet.getRdLocation(_pos.second + 1), _pos.second + 1};
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
    int vEnd = dfnhb[v][0];
    for (auto e : dfnhb[v]) {
      if (vEnd >= _pos.second)
        break;
      if (e - vEnd > 1)
        vStart = e;
      vEnd = e;
    }
    vector<int> possibleEvents;
    vector<float> w2;
    float tk1 = tnet.getEventVal(_pos.second + 1);
    for (int l = _pos.second + 1; l <= vEnd; l++) {
      possibleEvents.push_back(l);
      float tl = tnet.getEventVal(l);
      float w = 0.;
      for (int i = _pos.second + 1; i < l; i++) {
        if (i <= uEnd && vStart <= i &&
            tnet.checkEdgeAtEvent(_pos.first, v, i) &&
            tnet.checkEdgeAtEvent(_pos.first, v, i + 1)) {
          float ti1 = tnet.getEventVal(i + 1);
          float ti = tnet.getEventVal(i);
          float tiEnd = tnet.nodeVanishT(v, ti);
          float div = integral(ti - tk1, tiEnd - tk1, h, 100);
          w += (ti1 - ti) / div;
        }
      }
      float tl1 = tnet.getEventVal(vEnd + 1);
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