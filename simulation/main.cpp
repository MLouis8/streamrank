#include "include/fileHandler.hpp"
#include "include/matrix.hpp"
#include "include/network.hpp"
#include "include/pagerank.hpp"
#include "include/randomWalk.hpp"
#include "include/temporalNetwork.hpp"
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
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
 * Experimenting approx vs discrete steps
 */
void approComputation() {}

/**
 * Generate a temporal network and saves it
 */
void genTempoNet(int n, int sumNodes, float p, int nbEvents, string name,
                 float tStart, float tEnd, bool edge) {
  cout << "Temporal network parameters:\n";
  cout << "n: " << n << ", p: " << p;
  cout << ", sumNodes: " << sumNodes << '\n';
  tempoNetwork tnet(n, sumNodes, nbEvents, p, tStart, tEnd, edge);
  writeTempoNetwork(tnet, name);
  cout << "\nTemporal Network generated\n";
}

/**
 * Overlapping part experiment to see if negligeable or not
 */
void overlappingExperiment(tempoNetwork &rdTnet, int nbWalkers, int nbSteps,
                           float alpha) {
  // part 1 on basic link stream
  vector<vector<int>> nodeEvents1 = {{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1},
                                     {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}};
  vector<vector<pair<int, int>>> edgeEvents1 = {
      {{0, 1}}, {{0, 1}}, {{0, 1}}, {{0, 1}}, {{0, 1}},
      {{0, 1}}, {{0, 1}}, {{0, 1}}, {{0, 1}}, {{0, 1}}};
  tempoNetwork tnet1(0, 10, 2, nodeEvents1, edgeEvents1);
  // auto h = [](float x) { return x; };
  auto h = [](float x) { return exp(-x); };
  vector<vector<pair<int, int>>> walk1 =
      randomWalkSimulation(1000, 10, 1, tnet1, h, 2);
  float mean1 = meanOverlapping(walk1);
  cout << "\nOverlapping proportion for exp 1 is: " << mean1;

  vector<vector<pair<int, int>>> walk2 =
      randomWalkSimulation(nbWalkers, nbSteps, alpha, rdTnet, h, 2);
  float mean2 = meanOverlapping(walk2);
  cout << "\nOverlapping proportion for exp 2 is " << mean2;
  // for (int i = 0; i < walk2[0].size(); i++)
  //   cout << "(" << walk2[0][i].first << ", " << walk2[0][i].second << "); ";
}

/**
 * Real data set experiment to track the evolution on real stream graph
 */
void simulation() {}

int main(int argc, char *argv[]) {
  // genTempoNet(20, 160, 0.1, 10, "test2", 0., 100., false);
  // tempoNetwork rdTnet = readStreamFile("test2.stream");

  for (int j = 3; j < 4; j++) {
    for (int i = 68000; i < 80000; i += 400) {
      genTempoNet(800, i, 0.1, 100,
                  to_string(j) + "_800_" + to_string(i) + "_01_100", 0., 100.,
                  false);
    }
  }
  // Génération aléatoire de stream graph:

  // Now create a map assigning to graph name to the avg chunk node and avg
  // chunk edge For this choose 10 graphs in each categories

  // Then for all graphs do random walks with the following parameters:
  // 10000 walkers; 100 steps;

  // Compute the avgchunk size and find 100 batchs of 10 graphs each of
  // similar avgchunksize Compute the avg overlapping proportion in each
  // batch and render a figure

  // cout << "Avg edge Chunk size is " << rdTnet.avgEdgeChunkSize();
  // overlappingExperiment(rdTnet, 100, 10, 0.85);
  // overlappingExperiment(rdTnet, 100, 10, 1.);
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