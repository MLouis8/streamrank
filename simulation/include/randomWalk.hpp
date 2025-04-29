#ifndef RANDOMWALK_HPP
#define RANDOMWALK_HPP

#include "network.hpp"

std::vector<std::vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps, float eps, float alpha, Network net);

#endif // RANDOMWALK_HPP