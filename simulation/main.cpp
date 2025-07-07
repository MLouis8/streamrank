#include "include/fileHandler.hpp"
#include "include/matrix.hpp"
#include "include/network.hpp"
#include "include/pagerank.hpp"
#include "include/randomWalk.hpp"
#include "include/rdLib.hpp"
#include "include/temporalNetwork.hpp"
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

// vector<int> xadj = {0,  5,  8,  10, 13, 15, 18, 20,
//                     23, 24, 26, 27, 29, 31, 33, 34};
// vector<int> adj = {1,  2, 3, 4, 5, 0, 7, 9, 0, 9, 0, 10, 11, 0,  6,  0,
//                    12, 4, 7, 1, 6, 8, 1, 2, 3, 3, 5, 5,  13, 12, 14, 14};
// vector<int> adjWt = {9, 9, 9, 9, 9, 9, 7, 10, 9,  7, 9, 2, 9, 9, 6, 9, 6,
//                      5, 6, 8, 7, 8, 9, 9, 7,  10, 2, 9, 6, 5, 8, 8, 3, 3};
// vector<int> ndWt = {
//     10, 10, 9, 10, 10, 8, 7,
// };
// Network agloNet = Network(xadj, adj, adjWt, ndWt);

/**
 * Experimenting if the approx step converges towards a Pagerank computation on
 * certain temporal networks
 */
void expPRConvergence(int n, float pEdge, float alpha, float eps, int nbSteps,
                      int nbWalkers) {
  // Network creation
  Network net(pEdge, n);
  net.display();
  net.checkConsistency();
  vector<vector<int>> rdWalk =
      randomWalkSimulation(nbWalkers, nbSteps, eps, alpha, net);
  displayResults(rdWalk, net.size());
  int k = rdWalk[0].size() - 1;
  vector<float> res = walkersDistribution(rdWalk, k, net.size());

  // Pagerank computation
  vector<float> pValues(net.size(), 1. / net.size());
  Matrix p(pValues, {1, net.size()});
  pair<Matrix, Matrix> ha = networkToPagerakMatrices(net);
  Matrix pr = pwrPagerank(ha.first, ha.second, alpha, p, nbSteps, eps);
  cout << "Pagerank:\n";
  pr.print();

  cout << "Difference:\n";
  vector<float> diff;
  for (int i = 0; i < pr.dim().second; i++)
    diff.push_back(abs(pr(0, i) - res[i]));
  cout << accumulate(diff.begin(), diff.end(), 0.);
}

/**
 * Experimenting if the approx step stays in between the bounds defined for the
 * discrete step
 */
void expApproxBounds() {}

/**
 * Experimenting if the approx convergences towards the discrete step
 */
void approComputation() {}

/**
 * Generate a temporal network and saves it
 */
void genTempoNet(int n, int sumNodes, float p, int nbEvents, string name) {
  cout << "Graph parameters:\n";
  cout << "n: " << n << ", p: " << p;
  cout << ", sumNodes: " << sumNodes << '\n';
  tempoNetwork tnet(n, sumNodes, 10, 0.5);
  writeTempoNetwork(tnet, name);
  cout << "\nTemporal Network generated\n";
}

int main(int argc, char *argv[]) {
  genTempoNet(10, 70, 0.6, 10, "test0");
  // int nbWalkers = 1;
  // int nbSteps = 10; // 100;
  // float eps = 0.00001;
  // float alpha = 0.85;
  // tempoNetwork tnet = readStreamFile("../data/test.stream");
  // cout << "\nStream graph loaded\n";
  // auto h = [](float x) { return exp(-x); };
  // vector<vector<int>> rdWalk =
  //     randomWalkSimulation(nbWalkers, nbSteps, eps, alpha, tnet, h, 1);

  // cout << "\nRandom walk done, now the results:\n";
  // displayResults(rdWalk, tnet.size(), tnet.getNodeEvents());
  // cout << "\nNodes present at last event: ";
  // for (auto node : tnet.getNodeEvents()[tnet.getNodeEvents().size() -
  // 1])
  //   cout << node << " ";
  // cout << '\n';
  // cout << "\nWalkers positions at last event: ";
  // for (auto walker : rdWalk)
  //   cout << walker[walker.size() - 1] << " ";
  // cout << '\n';
  return 0;
}