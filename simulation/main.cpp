#include "include/fileHandler.hpp"
#include "include/matrix.hpp"
#include "include/network.hpp"
#include "include/pagerank.hpp"
#include "include/randomWalk.hpp"
#include "include/temporalNetwork.hpp"
#include <cmath>
#include <iostream>
#include <numeric>
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
                 float tStart, float tEnd) {
  cout << "Graph parameters:\n";
  cout << "n: " << n << ", p: " << p;
  cout << ", sumNodes: " << sumNodes << '\n';
  tempoNetwork tnet(n, sumNodes, 10, 0.5, tStart, tEnd, true);
  writeTempoNetwork(tnet, name);
  cout << "\nTemporal Network generated\n";
}

/**
 * Overlapping part experiment to see if negligeable or not
 */
void overlappingExperiment(tempoNetwork &rdTnet, int nbWalkers, int nbSteps,
                           float alpha) {
  // part 1 on basic link stream
  // vector<vector<int>> nodeEvents1 = {{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1},
  //                                    {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}};
  // vector<vector<pair<int, int>>> edgeEvents1 = {
  //     {{0, 1}}, {{0, 1}}, {{0, 1}}, {{0, 1}}, {{0, 1}},
  //     {{0, 1}}, {{0, 1}}, {{0, 1}}, {{0, 1}}, {{0, 1}}};
  // tempoNetwork tnet1(0, 10, 2, nodeEvents1, edgeEvents1);
  // auto h = [](float x) { return x; };
  auto h = [](float x) { return exp(-x); };
  // vector<vector<pair<int, int>>> walk1 =
  //     randomWalkSimulation(1000, 10, 1, tnet1, h, 2);
  // float mean1 = meanOverlapping(walk1);
  // cout << '\n' << mean1;

  vector<vector<pair<int, int>>> walk2 =
      randomWalkSimulation(nbWalkers, nbSteps, alpha, rdTnet, h, 2);
  // for (int i = 0; i < 10; i++) {
  // cout << "\nWalker " << i << " ended the walk in node ("
  //  << walk2[i].back().first << ", " << walk2[i].back().second
  //  << ") and walked " << walk2[i].size() << " steps.";
  // }
  // cout << "\nsize: " << walk2.size();
  // float mean2 = meanOverlapping(walk2);
  // cout << '\n'; // << mean2;
  // for (int i = 0; i < walk2[0].size(); i++)
  //   cout << "(" << walk2[0][i].first << ", " << walk2[0][i].second << "); ";
}

/**
 * Real data set experiment to track the evolution on real stream graph
 */
void simulation() {}

int main(int argc, char *argv[]) {
  // genTempoNet(100, 16000, 0.1, 200, "test", 0., 200.);
  tempoNetwork rdTnet = readStreamFile("test.stream");
  // int i = 0;
  // for (auto event : rdTnet.getNodeEvents()) {
  //   cout << "\nPour l'event " << i << " on a: ";
  //   for (auto node : event) {
  //     cout << node << ' ';
  //   }
  //   i += 1;
  // }
  // int j = 0;
  // for (auto event : rdTnet.getEdgeEvents()) {
  //   cout << "\nPour l'event " << j << " on a: ";
  //   for (auto edge : event) {
  //     cout << " (" << edge.first << ", " << edge.second << ')';
  //   }
  //   j += 1;
  // }
  // Génération aléatoire de stream graph:
  // Faire un paramaètre style compacité pour les arêtes

  // Puis quand on fait les arêtes, on prend deux chuncks qui se superposent, on
  // a une proba p qu'une arête existe entre ces deux chuncks puis on regarde la
  // taille de l'intersection des chuncks et on tire au hasard une partition de
  // cette taille la dans l'ensemble des partitions de {1, ..., taille inter}

  // Zero compute overlapping proportion on the existing graph and veriy
  // everything works
  // First generate 100 graphs with characteristics as the
  // first one Second compute avg overlapping proportion and compare with the
  // first result Third compute the 9900 last graphs create a curve of avg
  // ovelapping according to compacity

  // Compute the avgchunk size and find 100 batchs of 10 graphs each of similar
  // avgchunksize Compute the avg overlapping proportion in each batch and
  // render a figure

  cout << "Avg Chunk size is " << rdTnet.avgChunkSize();
  overlappingExperiment(rdTnet, 1, 10, 1.);
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