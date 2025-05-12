#ifndef PAGERANK_HPP
#define PAGERANK_HPP

#include "network.hpp"
#include <utility>
#include <vector>

using namespace std;

/**
 * Function taking a graph and returning the corresponding transition matrix and
 * dangling node vector.
 */
pair<vector<vector<float>>, vector<float>>
networkToPagerakMatrices(Network &net);

/**
 * Function computing the pagerank value using the classic power method.
 *
 * @parameter:
 *  h: transition matrix
 *  a: dangling node vector
 *  alpha: jumping probability
 *  p: personalization vector
 *  iter, eps: convergence criteria, the computation stops when the first one is
 * met
 */
vector<float> pwrPagerank(vector<vector<float>> &h, vector<float> &a,
                          float alpha, vector<float> &p, int iter, float eps);

#endif // PAGERANK_HPP
