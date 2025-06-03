#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <string>

#include "temporalNetwork.hpp"

tempoNetwork readStreamFile(const std::string &filename);

void writeTempoNetwork(tempoNetwork &tnet, string filename);

#endif // FILEHANDLER_HPP