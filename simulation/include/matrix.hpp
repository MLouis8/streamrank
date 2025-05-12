#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <utility>
#include <vector>

class matrix {
public:
  matrix(std::vector<std::vector<float>> data);
  matrix(std::vector<float> data);

  std::pair<int, int> dim() { return _dim; }
  float &operator()(unsigned row, unsigned col) {
    return data_[dim_ * row + col];
  }

  int operator()(unsigned row, unsigned col) const {
    return data_[dim_ * row + col];
  }

  void print();
  matrix operator+(matrix b);
  matrix operator-(matrix b);

private:
  std::pair<int, int> _dim;
  std::vector<float> _data;
};

#endif // MATRIX_HPP