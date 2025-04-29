#include "include/network.hpp"
#include <random>
#include <map>

std::vector<int> Network::getNeighbours(int u)
{
    std::vector<int> neighbours;
    for (int i = _xadjacency[u]; i < _xadjacency[u + 1]; i++)
        neighbours.push_back(_adjacency[i]);
    return neighbours;
}

std::vector<float> Network::getNeighboursWeights(int u)
{
    std::vector<float> weights;
    for (int i = _xadjacency[u]; i < _xadjacency[u + 1]; i++)
        weights.push_back(_adjacencyWeight[i]);
    return weights;
}

int Network::getRdLocation()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, _n);
    return dis(gen);
}

Network &randomErdosRenyiNetwork(int n, float p)
{
    std::vector<int> xadjacency;
    std::vector<float> adjacencyWeight;
    std::vector<int> adjacency;

    xadjacency.assign(n + 1, 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::map<std::pair<int, int>, bool> notSet;
    for (int i = 0; i < n; i++)
    {
        int x = 0;
        for (int j = 0; j < n && i != j; j++)
        {
            if (i < j)
            {
                if (dis(gen) <= p)
                {
                    adjacency.push_back(j);
                    notSet[{i, j}] = true;
                    x++;
                }
                else
                {
                    notSet[{i, j}] = false;
                }
            }
            else
            {
                if (notSet[{j, i}])
                {
                    adjacency.push_back(j);
                }
            }
        }
        xadjacency[i + 1] = xadjacency[i] + x;
    }
    static Network res = Network(xadjacency, adjacencyWeight, adjacency);
    return res;
}