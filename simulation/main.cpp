#include "include/network.hpp"
#include "include/randomWalk.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 5)
    throw std::invalid_argument("exec file must be called with arguments: "
                                "nbVertices nbWalkers probaEdge nbSteps");

  int nbVertices = std::atoi(argv[1]);  // 100;
  int nbWalkers = std::atoi(argv[2]);   // 1000;
  float probaEdge = std::atof(argv[3]); // 0.5;
  int nbSteps = std::atoi(argv[4]);     // 50;
  std::cout << nbVertices << " " << nbWalkers << " " << probaEdge << " "
            << nbSteps << std::endl;
  float eps = 0.0001;
  float alpha = 0.85;

  // Wheel of size 5
  //   std::vector<int> xadj0 = {0, 4, 5, 6, 7, 8};
  //   std::vector<int> adj0 = {1, 2, 3, 4, 0, 0, 0, 0};
  //   std::vector<float> adjWt0 = {1, 1, 1, 1, 1, 1, 1, 1};
  //   Network net0 = Network(xadj0, adj0, adjWt0);

  // K3
  //   std::vector<int> xadj1 = {0, 2, 4, 6};
  //   std::vector<int> adj1 = {2, 3, 1, 3, 1, 2};
  //   std::vector<float> adjWt1 = {1, 1, 1, 1, 1, 1};

  // K5
  std::vector<int> xadj2 = {0, 4, 8, 12, 16, 20};
  std::vector<int> adj2 = {1, 2, 3, 4, 0, 2, 3, 4, 0, 1,
                           3, 4, 0, 1, 2, 4, 0, 1, 2, 3};
  std::vector<float> adjWt2 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  Network net = randomErdosRenyiNetwork(nbVertices, probaEdge);

  // net.display();
  // net.checkConsistency();

  std::vector<std::vector<int>> res =
      randomWalkSimulation(nbWalkers, nbSteps, eps, alpha, net);

  displayResults(res, nbVertices);
  //   for (int w = 0; w < res.size(); w++) {
  //     std::cout << "Walker" << w << ": ";
  //     for (auto pos : res[w])
  //       std::cout << pos << " ";
  //     std::cout << std::endl;
  //   }
  return 0;
}