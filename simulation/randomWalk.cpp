#include "include/randomWalk.hpp"
#include "include/temporalNetwork.hpp"
#include "include/walker.hpp"
#include <functional>
#include <iostream>

#include <utility>
#include <vector>

vector<vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps, float eps,
                                         float alpha, Network &net) {
  vector<vector<int>> walkersPositions;
  vector<Walker<int>> walkersList;
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

// change random Location to have a starting position at time 0
// rmv while in actual rd Loc
vector<vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps, float eps,
                                         float alpha, tempoNetwork &tnet,
                                         function<float(float)> h) {
  vector<vector<int>> walkersPositions;
  vector<Walker<DTNode>> walkersList;
  for (int i = 0; i < nbWalkers; i++) {
    int startPos = tnet.getRdLocation(0);
    walkersPositions.push_back({startPos});
    walkersList.push_back(Walker<DTNode>(i, {startPos, 0}));
  }
  for (int s = 0; s < nbSteps; s++) {
    for (int i = 0; i < nbWalkers; i++) {
      if (walkersList[i].pos().second < tnet.nbEvents() - 1) {
        DTNode newLoc = walkersList[i].approxStep(tnet, alpha, h);
        walkersPositions[i].push_back(newLoc.first);
      }
    }
  }
  return walkersPositions;
}

vector<float> walkersDistribution(vector<vector<int>> wlkSteps, int step,
                                  int n) {
  vector<float> res(n, 0.);
  for (auto walker : wlkSteps) {
    res[walker[step]] += 1. / wlkSteps.size();
  }
  return res;
}

vector<float> walkersDistribution(vector<vector<int>> wlkSteps, int step, int n,
                                  vector<vector<int>> &nodeEvents) {
  vector<float> res(n, -1);
  for (int node : nodeEvents[step])
    res[node] = 0.;
  for (auto walker : wlkSteps) {
    res[walker[step]] += 1. / wlkSteps.size();
  }
  return res;
}

void displayResults(vector<vector<int>> rdWalk, int n) {
  int k = rdWalk[0].size() - 1;
  vector<float> res = walkersDistribution(rdWalk, k, n);
  cout << "Walkers Distribution: ";
  for (auto val : res)
    cout << " " << val;
  cout << '\n';
}

void displayResults(vector<vector<int>> rdWalk, int n,
                    vector<vector<int>> &nodeEvents) {
  int k = rdWalk[0].size() - 1;
  vector<float> res = walkersDistribution(rdWalk, k, n, nodeEvents);
  cout << "Walkers Distribution: ";
  for (auto val : res)
    cout << " " << val;
  cout << '\n';
}