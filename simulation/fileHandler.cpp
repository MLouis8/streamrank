#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "include/fileHandler.hpp"
#include "include/strHandler.hpp"

tempoNetwork readStreamFile(const std::string &filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::invalid_argument("Error opening file: " + filename);
  }

  // Read the first line: tStart and tEnd
  float tStart, tEnd;
  file >> tStart >> tEnd;

  // Read the second line: n
  int n;
  file >> n;

  // Read the next n lines into W
  std::vector<TimeItvs> W;
  W.assign(n, {});
  for (int i = 0; i < n; ++i) {
    int nodeId;
    file >> nodeId;

    std::string line;
    std::getline(file, line); // Read the rest of the line
    std::istringstream iss(line);
    float value1, value2;

    while (iss >> value1 >> value2) {
      W[nodeId].push_back({value1, value2});
    }
  }

  // Read the remaining lines into E
  std::unordered_map<std::string, TimeItvs> E;
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    int nodeId1, nodeId2;
    iss >> nodeId1 >> nodeId2;

    TimeItvs intervals;
    float value1, value2;

    while (iss >> value1 >> value2) {
      intervals.push_back({value1, value2});
    }

    E[pairToStr({nodeId1, nodeId2})] = intervals;
  }

  file.close();
  tempoNetwork net = tempoNetwork(tStart, tEnd, n, W, E);
  return net;
}

// int main() {
//   std::string filename = "example.stream";
//   StreamData data = readStreamFile(filename);

//   std::cout << "tStart: " << data.tStart << std::endl;
//   std::cout << "tEnd: " << data.tEnd << std::endl;
//   std::cout << "n: " << data.n << std::endl;

//   // Print W
//   std::cout << "W:" << std::endl;
//   for (int i = 0; i < data.n; ++i) {
//     std::cout << "Node " << i << ": ";
//     for (const auto &pair : data.W[i]) {
//       std::cout << "(" << pair.first << ", " << pair.second << ") ";
//     }
//     std::cout << std::endl;
//   }

//   // Print E
//   std::cout << "E:" << std::endl;
//   for (const auto &entry : data.E) {
//     std::cout << "Nodes (" << entry.first.first << ", " << entry.first.second
//               << "): ";
//     for (const auto &pair : entry.second) {
//       std::cout << "(" << pair.first << ", " << pair.second << ") ";
//     }
//     std::cout << std::endl;
//   }

//   return 0;
// }
