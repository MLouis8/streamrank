#ifndef RANDOMBIPARTITE_HPP
#define RANDOMBIPARTITE_HPP

#include <utility>
#include <vector>

using namespace std;

typedef pair<vector<vector<int>>, vector<vector<int>>> BipartiteGraph;

vector<int> rdPerm(int n);

BipartiteGraph rdBipartiteFromDegrees(vector<int> upDeg, vector<int> downDeg);

#endif // RANDOMBIPARTITE_HPP