#include "mesh.h"

Mesh::Mesh(const std::string s)
{
    file_path = s;
    positions = {
        -1.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
    };
    elementIndices = {
        0, 1, 2, 0, 2, 3
    };

    vertex_count = elementIndices.size();
}

Mesh::~Mesh() = default;