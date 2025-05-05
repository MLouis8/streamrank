#include "include/randomWalk.hpp"
#include "include/walker.hpp"
#include <iostream>
#include <vector>

std::vector<std::vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps,
                                                   float eps, float alpha,
                                                   Network &net) {
  std::vector<std::vector<int>> walkersPositions;
  std::vector<Walker<int>> walkersList;
  for (int i = 0; i < nbWalkers; i++) {
    int startingPosition = net.getRdLocation(-1);
    walkersPositions.push_back({startingPosition});
    walkersList.push_back(Walker<int>(i, startingPosition));
  }
  for (int s = 1; s < nbSteps; s++) {
    for (int i = 0; i < nbWalkers; i++) {
      int newLoc = walkersList[i].step(net, alpha);
      walkersPositions[i].push_back(newLoc);
    }
  }
  return walkersPositions;
}

std::vector<float> walkersDistribution(std::vector<std::vector<int>> steps,
                                       int k, int n) {
  std::vector<float> res;
  res.assign(n, 0.);
  for (auto walker : steps) {
    res[walker[k]] += 1. / steps.size();
  }
  return res;
}

void displayResults(std::vector<std::vector<int>> rdWalk, int n) {
  int k = rdWalk[0].size() - 1;
  std::vector<float> res = walkersDistribution(rdWalk, k, n);
  std::cout << "Pagerank Vector: ";
  for (auto val : res)
    std::cout << " " << val;
  std::cout << std::endl;
}