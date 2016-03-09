#ifndef STRINGHELPER_H
#define STRINGHELPER_H
#include <string>
#include <vector>
#include <iterator>

//# using namespace std;

typedef std::string::size_type (std::string::*find_t)(const std::string& delim,
                                                std::string::size_type offset) const;

std::vector<std::string> split(const std::string& s,
                         const std::string& match,
                         bool removeEmpty=false,
                         bool fullMatch=false);

#endif
