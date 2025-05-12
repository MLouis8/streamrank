#include "include/matrix.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

Matrix::Matrix(std::vector<std::vector<float>> data) {
  _dim = {data.size(), data.back().size()};
  _data = {};
  for (auto line : data) {
    for (float elem : line)
      _data.push_back(elem);
  }
}

void Matrix::print() {
  for (int i = 0; i < _dim.first; i++) {
    for (int j = 0; j < _dim.second; j++)
      std::cout << (*this)(i, j) << " ";
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

Matrix Matrix::operator+(Matrix b) {
  if (this->dim().first != b.dim().first ||
      this->dim().second != b.dim().second)
    throw std::invalid_argument("Same dimension matrices are required for "
                                "addition.");
  std::vector<float> data;
  for (int i = 0; i < _dim.first; i++)
    for (int j = 0; j < _dim.second; j++)
      data.push_back((*this)(i, j) + b(i, j));
  return Matrix(data, this->dim());
}

Matrix Matrix::operator-(Matrix b) {
  if (this->dim().first != b.dim().first ||
      this->dim().second != b.dim().second)
    throw std::invalid_argument(
        "Same dimension matrices are required for substraction.");
  std::vector<float> data;
  for (int i = 0; i < _dim.first; i++)
    for (int j = 0; j < _dim.second; j++)
      data.push_back((*this)(i, j) - b(i, j));
  return Matrix(data, this->dim());
}

Matrix Matrix::operator*(Matrix b) {
  if (this->dim().second != b.dim().first)
    throw std::invalid_argument(
        "Matrices must have a single common dimension for multiplication.");
  Matrix c(b.dim());
  for (int i = 0; i < c.dim().first; i++)
    for (int k = 0; k < c.dim().first; k++)
      for (int j = 0; j < c.dim().second; j++)
        c(i, j) += (*this)(i, k) * b(k, j);
  return c;
}

Matrix Matrix::operator+(float x) {
  std::vector<float> data;
  for (int i = 0; i < _dim.first; i++)
    for (int j = 0; j < _dim.second; j++)
      data.push_back((*this)(i, j) + x);
  return Matrix(data, this->dim());
}

Matrix Matrix::operator-(float x) {
  std::vector<float> data;
  for (int i = 0; i < _dim.first; i++)
    for (int j = 0; j < _dim.second; j++)
      data.push_back((*this)(i, j) - x);
  return Matrix(data, this->dim());
}

Matrix Matrix::operator*(float x) {
  std::vector<float> data;
  for (int i = 0; i < _dim.first; i++)
    for (int j = 0; j < _dim.second; j++)
      data.push_back((*this)(i, j) * x);
  return Matrix(data, this->dim());
}
