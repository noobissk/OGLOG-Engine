
#pragma once
#include <vector>
#include <string>

struct Mesh {
public:
    std::string file_path;
    Mesh(std::string s);
    ~Mesh();
    std::vector<float> positions;
    std::vector<unsigned int> elementIndices;
    unsigned int vertex_count;
};