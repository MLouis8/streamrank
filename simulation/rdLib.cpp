#include "include/rdLib.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

vector<int> rdPerm(int n) {
  random_device rd;
  mt19937 gen(rd());
  vector<int> res;
  for (int i = 0; i < n; i++)
    res.push_back(i);
  for (int i = n - 1; i > 0; i--) {
    std::uniform_int_distribution<> dis(0, i);
    int j = dis(gen);
    int temp = res[i];
    res[i] = res[j];
    res[j] = temp;
  }
  return res;
}

Bipartite rdBipartiteFromDegrees(vector<int> upDeg, vector<int> downDeg) {
  int sumDeg = accumulate(upDeg.begin(), upDeg.end(), 0);
  vector<int> perm = rdPerm(sumDeg);
  vector<int> upDegToNode;
  for (int u = 0; u < upDeg.size(); u++) {
    for (int i = 0; i < upDeg[u]; i++)
      upDegToNode.push_back(u);
  }
  vector<int> downDegToNode;
  for (int v = 0; v < downDeg.size(); v++) {
    for (int i = 0; i < downDeg[v]; i++)
      downDegToNode.push_back(v);
  }
  Bipartite res;
  for (int p = 0; p < perm.size(); p++) {
    res.push_back({upDegToNode[p], downDegToNode[perm[p]]});
  }
  sort(res.begin(), res.end());
  res.erase(unique(res.begin(), res.end()), res.end());
  return res;
}

vector<int> rdTimeSeries(int sum, int nbEvents, int nbObj) {
  vector<int> res(nbEvents, 0);
  random_device rd;
  mt19937 gen(rd());
  vector<int> cap(nbEvents, nbObj);
  vector<int> w(nbEvents, 1);
  discrete_distribution<> dis(w.begin(), w.end());
  for (int i = 0; i < sum; i++) {
    int id = dis(gen);
    res[id]++;
    cap[id]--;
    if (cap[id] == 0) {
      w[id] = 0;
      dis = discrete_distribution<>(w.begin(), w.end());
    }
  }
  return res;
}
