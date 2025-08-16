#include "triangle_mesh.h"
#include <vector>
#include <glad/glad.h>

TriangleMesh::TriangleMesh()
{
    std::vector<float> positions = {
        -1.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
    };

    std::vector<unsigned int> vertexColors = {
        1, 3, 0, 2
    };

    std::vector<unsigned int> elementIndices = {
        0, 1, 2, 0, 2, 3
    };


    vertex_count = elementIndices.size();

    glGenVertexArrays(1, &VAO); // VAO records all buffer handling below
    glBindVertexArray(VAO);

    VBOs.resize(2);

    glGenBuffers(2, VBOs.data());


    
    // position as set up in the shader
    // 24 indicating the number of bytes we need to read through to get to another attribute (6 floats(4 bytes) = 24)
    // (void*)0 explains to the gpu the offset where we should start reading the attrib
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(0);
    
    // color as set up in the shader
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, vertexColors.size() * sizeof(unsigned int), vertexColors.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(1, 1, GL_INT, sizeof(unsigned int), (void*)0);
    glEnableVertexAttribArray(1);

    // color as set up in the shader
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementIndices.size() * sizeof(unsigned int), elementIndices.data(), GL_STATIC_DRAW);
}

void TriangleMesh::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteBuffers(1, &EBO);
}