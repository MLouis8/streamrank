#ifndef RANDOMWALK_HPP
#define RANDOMWALK_HPP

#include "network.hpp"
#include "temporalNetwork.hpp"
#include <functional>
#include <utility>
#include <vector>

using namespace std;

/**
 * Simulation of a random walk on a static network (graph).
 */
vector<vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps, float eps,
                                         float alpha, Network &net);

vector<vector<pair<int, int>>> randomWalkSimulation(int nbWalkers, int nbSteps,
                                                    float alpha,
                                                    tempoNetwork &tnet,
                                                    function<float(float)> h,
                                                    int stepType);

vector<float> walkersDistribution(vector<vector<int>> wlkSteps, int step,
                                  int n);

vector<float> walkersDistribution(vector<vector<int>> wlkSteps, int step, int n,
                                  vector<vector<int>> &nodeEvents);

void displayResults(vector<vector<int>> rdWalk, int n);

void displayResults(vector<vector<int>> rdWalk, int n,
                    vector<vector<int>> &nodeEvents);

float meanOverlapping(vector<vector<pair<int, int>>> walk);

vector<vector<float>> streamRank(vector<vector<pair<int, int>>> walk, int n,
                                 int nbEvents);

#endif // RANDOMWALK_HPP