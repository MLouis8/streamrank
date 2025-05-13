#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <utility>
#include <vector>

/**
 * Matrix class in order to do simple matrix multiplications for pagerank
 * computation
 */
class Matrix {
public:
  Matrix(std::pair<int, int> dim) : _dim(dim) {
    _data.assign(dim.first * dim.second, 0.);
  }
  Matrix(std::vector<std::vector<float>> data);
  Matrix(std::vector<float> data, std::pair<int, int> dim)
      : _dim(dim), _data(data) {}

  std::pair<int, int> dim() { return _dim; }
  float &operator()(int row, int col) { return _data[_dim.second * row + col]; }
  float operator()(int row, int col) const {
    return _data[_dim.second * row + col];
  }

  void print();
  Matrix operator+(Matrix b);
  Matrix operator-(Matrix b);
  Matrix operator*(Matrix b);

  Matrix operator+(float x);
  Matrix operator-(float x);
  Matrix operator*(float x);

private:
  std::pair<int, int> _dim;
  std::vector<float> _data;
};

#endif // MATRIX_HPP