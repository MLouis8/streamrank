#ifndef RANDOMWALK_HPP
#define RANDOMWALK_HPP

#include "network.hpp"
#include "temporalNetwork.hpp"
#include <string>

std::vector<std::vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps,
                                                   float eps, float alpha,
                                                   Network &net);

template <typename Location>
std::vector<std::vector<Location>>
randomWalkSimulation(int nbWalkers, int nbSteps, float eps, float alpha,
                     tempoNetwork &tnet, std::string step);

std::vector<float> walkersDistribution(std::vector<std::vector<int>> steps,
                                       int k, int n);

void displayResults(std::vector<std::vector<int>> rdWalk, int n);

#endif // RANDOMWALK_HPP