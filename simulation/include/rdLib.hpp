#ifndef RANDOMBIPARTITE_HPP
#define RANDOMBIPARTITE_HPP

#include <utility>
#include <vector>

using namespace std;

typedef vector<pair<int, int>> Bipartite;

vector<int> rdPerm(int n);

Bipartite rdBipartiteFromDegrees(vector<int> upDeg, vector<int> downDeg);

vector<int> rdTimeSeries(int sum, int nbEvents, int nbObj);

#endif // RANDOMBIPARTITE_HPP