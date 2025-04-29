#include "include/randomWalk.hpp"
#include "include/walker.hpp"

int main()
{
    int nbVertices = 10000;
    int nbWalkers = 20;
    float probaEdge = 0.5;

    int nbSteps = 10;
    float eps = 0.0001;
    float alpha = 0.85;

    Network net = randomErdosRenyiNetwork(nbVertices, probaEdge);

    std::vector<std::vector<int>> res = randomWalkSimulation(nbWalkers, nbSteps, eps, alpha, net);
    return 0;
}