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

/**
 * Experimenting if the approx step converges towards a Pagerank computation on
 * certain temporal networks
 */
void expPRConvergence() {}

/**
 * Experimenting if the approx step stays in between the bounds defined for the
 * discrete step
 */
void expApproxBounds() {}

/**
 * Experimenting if the approx convergences towards the discrete step
 */
void approComputation() {}

int main(int argc, char *argv[]) {

  int n = 10;
  int nbWalkers = 10000;
  float p1 = 0.2;
  int nbSteps = 10; // 100;
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
  auto h = [](float x) { return std::exp(-x); };
  tempoNetwork tnet = randomTempoNetwork(n, tStart, tEnd, p1);
  std::vector<std::vector<std::pair<int, int>>> rdWalk =
      randomWalkSimulation(nbWalkers, nbSteps, eps, alpha, tnet, h, 1);

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