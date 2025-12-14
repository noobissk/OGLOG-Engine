
#pragma once
#include <vector>
#include <string>

struct Mesh {
public:
    std::string file_path;
    std::vector<float> positions;
    std::vector<unsigned int> element_indices;
    unsigned int index_count;
    
    unsigned int VBO, VAO, EBO;

    Mesh(const std::string& s);
    Mesh(std::vector<float>& _positions, std::vector<unsigned int>& _element_indices);

    void updateMesh(std::vector<float>& _positions, std::vector<unsigned int>& _element_indices);
    ~Mesh();
private:
    void sendMeshToGPU();
    void generateDefaultMesh();
};