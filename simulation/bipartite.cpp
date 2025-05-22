#include "include/bipartite.hpp"
#include <algorithm>
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