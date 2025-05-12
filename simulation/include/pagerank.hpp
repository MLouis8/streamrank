#ifndef PAGERANK_HPP
#define PAGERANK_HPP

#include "matrix.hpp"
#include "network.hpp"
#include <utility>

/**
 * Function taking a graph and returning the corresponding transition matrix and
 * dangling node vector.
 */
std::pair<Matrix, Matrix> networkToPagerakMatrices(Network &net);

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
Matrix pwrPagerank(Matrix &h, Matrix &a, float alpha, Matrix &p, int iter,
                   float eps);

#endif // PAGERANK_HPP
