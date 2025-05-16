#include "include/matrix.hpp"
#include "include/network.hpp"
#include "include/pagerank.hpp"
#include "include/randomWalk.hpp"
// #include "include/temporalNetwork.hpp"
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

// Wheel of size 5
// std::vector<int> xadj0 = {0, 4, 5, 6, 7, 8};
// std::vector<int> adj0 = {1, 2, 3, 4, 0, 0, 0, 0};
// std::vector<float> adjWt0 = {1, 1, 1, 1, 1, 1, 1, 1};
// Network net0(xadj0, adj0, adjWt0);

// K3
// std::vector<int> xadj1 = {0, 2, 4, 6};
// std::vector<int> adj1 = {2, 3, 1, 3, 1, 2};
// std::vector<float> adjWt1 = {1, 1, 1, 1, 1, 1};
// Network net1(xadj1, adj1, adjWt1);

//   for (int w = 0; w < res.size(); w++) {
//     std::cout << "Walker" << w << ": ";
//     for (auto pos : res[w])
//       std::cout << pos << " ";
//     std::cout << std::endl;
//   }

// K5
// std::vector<int> xadj2 = {0, 4, 8, 12, 16, 20};
// std::vector<int> adj2 = {1, 2, 3, 4, 0, 2, 3, 4, 0, 1,
//                          3, 4, 0, 1, 2, 4, 0, 1, 2, 3};
// std::vector<float> adjWt2 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//                              1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
// Network net2(xadj2, adj2, adjWt2);

int main(int argc, char *argv[]) {

  int n = 10;
  int nbWalkers = 10000;
  float p1 = 0.2;
  int nbSteps = 100;
  float tStart = 0.;
  float tEnd = 10.;
  // float p2 = 0.75;
  // float p3 = 0.9;

  std::cout << "Graph parameters:" << std::endl;
  std::cout << "n: " << n << ", P(edge): " << p1;
  std::cout << ", t_start: " << tStart << ", t_end: " << tEnd << std::endl;
  // std::cout << "Avg node filling: " << p2 << ", Avg edge filling: " << p3
  // << std::endl;
  std::cout << "Random Walk parameters:" << std::endl;
  std::cout << "nb walkers: " << nbWalkers << ", nb steps:" << nbSteps
            << std::endl;
  float eps = 0.00001;
  float alpha = 0.85;

  // Static network generation + pr sim
  // Network net = randomErdosRenyiNetwork(nbVertices, probaEdge);
  // net.display();
  // net.checkConsistency();
  // std::vector<std::vector<int>> rdWalk =
  //     randomWalkSimulation(nbWalkers, nbSteps, eps, alpha, net);
  // displayResults(rdWalk, net.size());

  // int k = rdWalk[0].size() - 1;
  // std::vector<float> res = walkersDistribution(rdWalk, k, net.size());

  // Temporal network generation
  tempoNetwork tnet = randomTempoNetwork(n, tStart, tEnd, p1); //, p2, p3);
  // std::vector<std::vector<int>> tres = randomWalkSimulation();

  // Pagerank computation
  // std::vector<float> pValues(net.size(), 1. / net.size());
  // Matrix p(pValues, {1, net.size()});
  // std::pair<Matrix, Matrix> ha = networkToPagerakMatrices(net);
  // Matrix pr = pwrPagerank(ha.first, ha.second, alpha, p, nbSteps, eps);
  // std::cout << "Pagerank:" << std::endl;
  // pr.print();

  // std::cout << "Difference:" << std::endl;
  // std::vector<float> diff;
  // for (int i = 0; i < pr.dim().second; i++)
  //   diff.push_back(std::abs(pr(0, i) - res[i]));
  // std::cout << accumulate(diff.begin(), diff.end(), 0.);
  return 0;
}