#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

void overlapBarplot(float p1) {
  plt::bar({1, 2}, {p1 * 10, (1 - p1) * 10});
  plt::legend();
  plt::title("Overlapping proportion");
  plt::save("./test.pdf");
}