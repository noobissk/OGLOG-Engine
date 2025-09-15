#pragma once
#include <vector>


class Mesh {
public:
    std::vector<float> vertexPositions;

    std::vector<unsigned int> vertexColors;

    std::vector<unsigned int> elementIndices;

    unsigned int vertexCount = elementIndices.size();
};