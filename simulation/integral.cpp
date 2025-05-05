#include "include/integral.hpp"

float integral(float s, float t, std::function<float(float)> h, int precision) {
  float res = 0;
  float step = (t - s) / precision;
  if (h(s) >= h(t)) { // decroissante
    for (float x = s; x + step < t; x += step) {
      res += h(x + step) * step;
    }
  } else {
    for (float x = s; x + step < t; x += step) {
      res += h(x) * step;
    }
  }
  return res;
}