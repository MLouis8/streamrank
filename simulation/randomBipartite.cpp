#include "include/randomBipartite.hpp"
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

BipartiteGraph rdBipartiteFromDegrees(vector<int> upDeg, vector<int> downDeg) {
  int sumDeg = accumulate(upDeg.begin(), upDeg.end(), 0);
  vector<int> perm = rdPerm(sumDeg);
  vector<vector<int>> up, down;
  for (int i = 0; i < perm.size(); i++) {
    up[i] = perm[i];
  }
  return {up, down};
}