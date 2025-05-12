#include "include/strHandler.hpp"
#include <sstream>

std::pair<int, int> strToPair(const std::string &key) {
  std::istringstream iss(key);
  std::pair<int, int> pair;
  std::string token;
  std::getline(iss, token, ';');
  pair.first = std::stoi(token);
  std::getline(iss, token, ';');
  pair.second = std::stoi(token);
  return pair;
}

std::string pairToStr(std::pair<int, int> pair) {
  return std::to_string(pair.first) + ";" + std::to_string(pair.second);
}