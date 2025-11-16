#pragma once
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <unordered_set>


std::pair<std::string, std::string> string_split(std::string line, char delimiter) {
    std::stringstream ss(line);
    std::pair<std::string, std::string> output;
    
    if (std::getline(ss, output.first, delimiter) && std::getline(ss, output.second)) {
        return output;
    }
    return output;
}

std::pair<std::string, std::string> string_split(std::string line, std::string delimiter) {
    std::pair<std::string, std::string> output;
    size_t pos = line.find(delimiter);
    if (pos != std::string::npos) {
        output.first = line.substr(0, pos);
        output.second = line.substr(pos + delimiter.length());
        return output;
    }
    return output;
}

std::vector<std::string> merge_unique_lines(const std::vector<std::string>& v1,
                                            const std::vector<std::string>& v2) {

    std::unordered_set<std::string> seen;

    for (const auto& s : v1)
        seen.insert(s);

    for (const auto& s : v2)
        seen.insert(s);

    return std::vector<std::string>(seen.begin(), seen.end());
}