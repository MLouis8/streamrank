#ifndef RANDOMBIPARTITE_HPP
#define RANDOMBIPARTITE_HPP

#include <utility>
#include <vector>

using namespace std;

typedef vector<pair<int, int>> Bipartite;

/**
 * Generates a random permutation of size n that takes the form
 * of a vector of size n of integers from 0 to n - 1
 */
vector<int> rdPerm(int n);

/**
 * Generates a random bipartite graph given two degrees distributions
 * May return a bipartite graph with approximately the same degrees as input
 */
Bipartite rdBipartiteFromDegrees(vector<int> upDeg, vector<int> downDeg);

/**
 * Generates a random time series in the form of an integer vector.
 * Index i contains the value of the time series at time i
 * @param
 *    sum is the sum of the vector values'
 *    nbEvents is the length of the time series
 *    nbObj is the upper bound of each single vector value
 */
vector<int> rdTimeSeries(int sum, int nbEvents, int nbObj);

#endif // RANDOMBIPARTITE_HPP