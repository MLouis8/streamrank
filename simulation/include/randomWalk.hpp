#ifndef RANDOMWALK_HPP
#define RANDOMWALK_HPP

#include "network.hpp"

std::vector<std::vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps, float eps, float alpha, Network& net);

std::vector<float> walkersDistribution(std::vector<std::vector<int>> steps, int k, int n);

#endif // RANDOMWALK_HPP