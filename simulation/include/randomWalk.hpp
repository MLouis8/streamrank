#ifndef RANDOMWALK_HPP
#define RANDOMWALK_HPP

#include <string>
#include "network.hpp"
#include "temporalNetwork.hpp"

std::vector<std::vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps, float eps, float alpha, Network& net);

template<typename Location> std::vector<std::vector<Location>> randomWalkSimulation(int nbWalkers, int nbSteps, float eps, float alpha, tempoNetwork& tnet, std::string step);

std::vector<float> walkersDistribution(std::vector<std::vector<int>> steps, int k, int n);

#endif // RANDOMWALK_HPP