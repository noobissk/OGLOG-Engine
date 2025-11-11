
#pragma once
#include <vector>
#include <string>

struct Mesh {
public:
    std::string file_path;
    std::vector<float> positions;
    std::vector<unsigned int> elementIndices;
    unsigned int index_count;
    
    unsigned int VBO, VAO, EBO;

    Mesh(const std::string& s);
    ~Mesh();
};