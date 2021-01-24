#include "stringutils.hpp"
#include <sstream>
#include <iostream>

void string_split(std::vector<std::string>& out, const std::string& in, char delim){
    std::stringstream ss(in);
    std::string item;
    while(std::getline(ss, item, delim)){
        out.push_back(item);
    }
}