#include "include/randomWalk.hpp"
#include "include/walker.hpp"

std::vector<std::vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps, float eps, float alpha, Network& net)
{
    int startingPosition = net.getRdLocation();
    std::vector<std::vector<int>> walkersPositions(nbWalkers, {startingPosition});
    std::vector<Walker<int>> walkersList;
    for (int i=0; i < nbWalkers; i++)
    {
        walkersList.push_back(Walker<int>(i, startingPosition));
    }
    for (int step=0; step < nbSteps; step++)
    {
        for (int i=0; i < nbWalkers; i++)
        {
            int newLoc = walkersList[i].step(net, alpha);
            walkersPositions[i].push_back(newLoc);
        }
    }
    return walkersPositions;
}

std::vector<float> walkersDistribution(std::vector<std::vector<int>> steps, int k, int n)
{
    std::vector<float> res;
    res.assign(0., n);
    for (auto walker: steps)
    {
        res[walker[k]] += 1. / steps.size();
    }
    return res;
}