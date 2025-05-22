#ifndef RANDOMBIPARTITE_HPP
#define RANDOMBIPARTITE_HPP

#include <utility>
#include <vector>

using namespace std;

typedef vector<pair<int, int>> Bipartite;

vector<int> rdPerm(int n);

Bipartite rdBipartiteFromDegrees(vector<int> upDeg, vector<int> downDeg);

#endif // RANDOMBIPARTITE_HPP