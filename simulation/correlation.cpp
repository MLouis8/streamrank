#include "include/correlation.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>

using namespace std;

float corrCoeff(vector<float> v1, vector<float> v2) {
  int n = v1.size();
  assert(n == v2.size());
  float sumV1 = 0., sumV2 = 0., sumV1V1 = 0., sumV2V2 = 0., sumV1V2 = 0.;
  for (int i = 0; i < n; i++) {
    sumV1 += v1[i];
    sumV2 += v2[i];
    sumV1V1 += v1[i] * v1[i];
    sumV2V2 += v2[i] * v2[i];
    sumV1V2 += v1[i] * v2[i];
  }
  return (n * sumV1V2 - sumV1 * sumV2) /
         sqrt((n * sumV1V1 - sumV1 * sumV1) * (n * sumV2V2 - sumV2 * sumV2));
}

vector<float> meanWalksCorr(vector<vector<float>> w1,
                            vector<vector<float>> w2) {
  vector<float> corcoefs;
  for (int i = 0; i < w1.size(); i++) {
    corcoefs.push_back(corrCoeff(w1[i], w2[i]));
  }
  float mean =
      accumulate(corcoefs.begin(), corcoefs.end(), 0.) / corcoefs.size();
  float maxCorr = *max_element(corcoefs.begin(), corcoefs.end());
  float minCorr = *min_element(corcoefs.begin(), corcoefs.end());
  return {mean, maxCorr, minCorr};
}