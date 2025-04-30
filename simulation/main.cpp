#include "include/network.hpp"
#include "include/randomWalk.hpp"
#include <iostream>

int main()
{
    int nbVertices = 10;
    int nbWalkers = 1;
    float probaEdge = 0.5;

    int nbSteps = 10;
    float eps = 0.0001;
    float alpha = 0.85;

    // Wheel of size 5
    std::vector<int> xadj0 = {0, 4, 5, 6, 7, 8};
    std::vector<int> adj0 = {1, 2, 3, 4, 0,0, 0, 0};
    std::vector<float> adjWt0 = {1, 1, 1, 1, 1, 1, 1, 1};
    Network net0 = Network(xadj0, adj0, adjWt0);

    // K3
    std::vector<int> xadj1 = {0, 2, 4, 6};
    std::vector<int> adj1 = {2, 3, 1, 3, 1, 2};
    std::vector<float> adjWt1 = {1, 1, 1, 1, 1, 1};

    // Network net = randomErdosRenyiNetwork(nbVertices, probaEdge);
    
    // for (int i=0 ; i < 10; i++) {
    //     std::vector<int> n = net.getNeighbours(i);
    //     std::cout << n.size();
    //     for (auto neigh : n)
    //         std::cout << neigh;
    //     std::cout << std::endl;
    // }

    std::vector<std::vector<int>> res = randomWalkSimulation(nbWalkers, nbSteps, eps, alpha, net0);

    for (int w = 0; w < res.size(); w++)
    {
        std::cout << "Walker" << w << ": ";
        for (auto pos: res[w])
            std::cout << pos << " ";
        std::cout << std::endl;
    }
    return 0;
}