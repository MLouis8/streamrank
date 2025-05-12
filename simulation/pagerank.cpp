#include "include/pagerank.hpp"
#include <cstdlib>
#include <numeric>
#include <vector>

pair<vector<vector<float>>, vector<float>>
networkToPagerakMatrices(Network &net) {
  vector<vector<float>> h;
  vector<float> a;
  a.assign(net.size(), 0.);
  for (int u = 0; u < net.size(); u++) {
    vector<int> neigh = net.neighbours(u);
    vector<float> hh;
    hh.assign(net.size(), 0.);
    if (neigh.size() == 0) {
      a[u] = 1.;
    } else {
      vector<float> neighW = net.neighboursWeights(u);
      float wghtS = accumulate(neighW.begin(), neighW.end(), 0.);
      for (int vIndex = 0; vIndex < neigh.size(); vIndex++) {
        hh[neigh[vIndex]] = neighW[vIndex] / wghtS;
      }
    }
    h.push_back(hh);
  }
  return {h, a};
}

vector<float> pwrPagerank(vector<vector<float>> &h, vector<float> &a,
                          float alpha, vector<float> &p, int iter, float eps) {
  vector<float> old = p;
  vector<float> pr;
  for (int it = 0; it < iter; it++) {

    ;
    vector<float> diff;
    for (int i = 0; i < old.size(); i++)
      diff.push_back(abs(pr[i] - old[i]));
    if (accumulate(diff.begin(), diff.end(), 0.) <= eps)
      return pr;
    old = pr;
  }
  return pr;
}