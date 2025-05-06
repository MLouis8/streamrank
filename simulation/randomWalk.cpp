#include "include/randomWalk.hpp"
#include "include/temporalNetwork.hpp"
#include "include/walker.hpp"
#include <functional>
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

std::vector<std::vector<std::pair<int, int>>>
randomWalkSimulation(int nbWalkers, int nbSteps, float eps, float alpha,
                     tempoNetwork &tnet, std::function<float(float)> h,
                     int stepType) {
  std::vector<std::vector<DTNode>> walkersPositions;
  std::vector<Walker<DTNode>> walkersList;
  for (int i = 0; i < nbWalkers; i++) {
    int startingPosition = tnet.getRdLocation(-1);
    walkersPositions.push_back({{startingPosition, 0}});
    walkersList.push_back(Walker<DTNode>(i, {startingPosition, 0}));
  }
  for (int s = 1; s < nbSteps; s++) {
    for (int i = 0; i < nbWalkers; i++) {
      DTNode newLoc;
      switch (stepType) {
      case 0: // DTRW
        newLoc = walkersList[i].step(tnet, alpha);
      case 1: // approx
        newLoc = walkersList[i].approxStep(tnet, alpha);
      case 2: // upper bound
        newLoc = walkersList[i].upperBound(tnet, alpha);
      case 3: // lower bound
        newLoc = walkersList[i].step(tnet, alpha);
      default: // approx
        newLoc = walkersList[i].lowerBound(tnet, alpha);
      }
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