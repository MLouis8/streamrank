#include "include/randomWalk.hpp"
#include "include/walker.hpp"

std::vector<std::vector<int>> randomWalkSimulation(int nbWalkers, int nbSteps, float alpha, Network net)
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