#ifndef RANDOMWALK_HPP
#define RANDOMWALK_HPP

#include "network.hpp"
#include "temporalNetwork.hpp"
#include <functional>
#include <utility>

std::vector<std::vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps,
                                                   float eps, float alpha,
                                                   Network &net);

std::vector<std::vector<std::pair<int, int>>>
randomWalkSimulation(int nbWalkers, int nbSteps, float eps, float alpha,
                     tempoNetwork &tnet, std::function<float(float)> h,
                     int stepType);

std::vector<float> walkersDistribution(std::vector<std::vector<int>> steps,
                                       int k, int n);

void displayResults(std::vector<std::vector<int>> rdWalk, int n);

#endif // RANDOMWALK_HPP