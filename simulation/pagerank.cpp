#include "include/pagerank.hpp"
#include "include/matrix.hpp"
#include <cstdlib>
#include <numeric>
#include <utility>
#include <vector>

std::pair<Matrix, Matrix> networkToPagerakMatrices(Network &net) {
  std::vector<std::vector<float>> h;
  std::vector<float> a(net.size(), 0.);
  for (int u = 0; u < net.size(); u++) {
    std::vector<int> neigh = net.neighbours(u);
    std::vector<float> hh(net.size(), 0.);
    if (neigh.size() == 0) {
      a[u] = 1.;
    } else {
      std::vector<float> neighW = net.neighboursWeights(u);
      float wghtS = accumulate(neighW.begin(), neighW.end(), 0.);
      for (int vIndex = 0; vIndex < neigh.size(); vIndex++) {
        hh[neigh[vIndex]] = neighW[vIndex] / wghtS;
      }
    }
    h.push_back(hh);
  }
  return {Matrix(h), Matrix(a, {a.size(), 1})};
}

Matrix pwrPagerank(Matrix &h, Matrix &a, float alpha, Matrix &p, int iter,
                   float eps) {
  Matrix old = p;
  Matrix pr(p.dim());
  for (int it = 0; it < iter; it++) {
    pr = (old * alpha) * h + p * (1 - alpha + ((old * alpha) * a)(0, 0));
    std::vector<float> diff;
    for (int i = 0; i < old.dim().second; i++)
      diff.push_back(std::abs(pr(0, i) - old(0, i)));
    if (accumulate(diff.begin(), diff.end(), 0.) <= eps)
      return pr;
    old = pr;
  }
  return pr;
}