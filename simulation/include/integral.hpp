#ifndef INTEGRAL_HPP
#define INTEGRAL_HPP

#include <functional>

/**
 * Integral square approximation
 */
float integral(float s, float t, std::function<float(float x)> h,
               int precision);
#endif // INTEGRAL_HPP