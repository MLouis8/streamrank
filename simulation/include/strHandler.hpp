#ifndef STRHANDLER_HPP
#define STRHANDLER_HPP

#include <string>

/**
 * Functions for translating back and forth _E keys from pair<int, int> to
 * string. The stirng format is 3;4
 */
std::pair<int, int> strToPair(const std::string &key);

std::string pairToStr(std::pair<int, int> pair);

#endif // STRHANDLER_HPP