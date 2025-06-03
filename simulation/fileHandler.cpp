#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "include/fileHandler.hpp"

using namespace std;

tempoNetwork readStreamFile(const string &filename) {
  ifstream ifile(filename);

  if (!ifile.is_open()) {
    throw invalid_argument("Error opening file: " + filename);
  }

  // Read the first line: tStart and tEnd
  float tStart, tEnd;
  ifile >> tStart >> tEnd;

  // Read the second line: n
  int n;
  ifile >> n;

  // Read the third line: nbEvents
  int nbEvents;
  ifile >> nbEvents;

  // Read the next nbEvents lines into _nodeEvents
  vector<vector<int>> nodeEvents;
  for (int i = 0; i < nbEvents; i++) {
    string line;
    vector<int> nodes;
    getline(ifile, line); // read the line
    istringstream iss(line);
    int node;
    while (iss >> node) {
      nodes.push_back(node);
    }
    nodeEvents.push_back(nodes);
  }

  // Read the next nbEvents lines into _edgeEvents
  vector<vector<pair<int, int>>> edgeEvents;
  for (int i = 0; i < nbEvents; i++) {
    string line;
    vector<pair<int, int>> edges;
    getline(ifile, line); // read the line
    istringstream iss(line);
    int n1, n2;
    while (iss >> n1 >> n2) {
      edges.push_back({n1, n2});
    }
    edgeEvents.push_back(edges);
  }

  ifile.close();
  tempoNetwork tnet = tempoNetwork(tStart, tEnd, n, nodeEvents, edgeEvents);
  return tnet;
}

void writeTempoNetwork(tempoNetwork &tnet, string filename) {
  ofstream ofile;
  ofile.open(filename + ".stream");
  ofile << tnet.startTime() << ' ' << tnet.endTime() << '\n';
  ofile << tnet.size() << '\n';
  ofile << tnet.nbEvents() << '\n';
  for (auto nodeEvent : tnet.getNodeEvents()) {
    for (auto e : nodeEvent)
      ofile << e << ' ';
    ofile << '\n';
  }
  for (auto edgeEvent : tnet.getEdgeEvents()) {
    for (auto e : edgeEvent)
      ofile << e.first << ' ' << e.second << ' ';
    ofile << '\n';
  }
  ofile.close();
}

// Read the next n lines into W
// vector<TimeItvs> W;
// W.assign(n, {});
// for (int i = 0; i < n; ++i) {
//   int nodeId;
//   file >> nodeId;

//   string line;
//   getline(file, line); // Read the rest of the line
//   istringstream iss(line);
//   float value1, value2;

//   while (iss >> value1 >> value2) {
//     W[nodeId].push_back({value1, value2});
//   }
// }

// // Read the remaining lines into E
// unordered_map<string, TimeItvs> E;
// string line;
// while (getline(file, line)) {
//   istringstream iss(line);
//   int nodeId1, nodeId2;
//   iss >> nodeId1 >> nodeId2;

//   TimeItvs intervals;
//   float value1, value2;

//   while (iss >> value1 >> value2) {
//     intervals.push_back({value1, value2});
//   }

//   E[pairToStr({nodeId1, nodeId2})] = intervals;
// }