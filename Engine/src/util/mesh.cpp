#include "mesh.h"
#include <config_render.h>
#include <iostream>


void Mesh::sendMeshToGPU() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), element_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
}

void Mesh::generateDefaultMesh() {
    positions = {
        // positions            // texture coords
        -0.5f,  0.5f,  0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.0f,    0.0f, 1.0f,
         0.5f,  0.5f,  0.0f,    0.0f, 0.0f,
    };
    element_indices = {
        0, 1, 2,
        0, 2, 3
    };
    index_count = element_indices.size(); // renamed
}

Mesh::Mesh(const std::string& s)
{
    file_path = s;
    if (!s.empty()) {
        // load from file
    }
    else {
        generateDefaultMesh();
    }


    sendMeshToGPU();
}
Mesh::Mesh(std::vector<float>& _positions, std::vector<unsigned int>& _element_indices)
{
    file_path = "";
    positions = std::move(_positions);
    element_indices = std::move(_element_indices);
    index_count = element_indices.size(); // renamed

    sendMeshToGPU();
}

void Mesh::updateMesh(std::vector<float>& _positions, std::vector<unsigned int>& _element_indices) {
    file_path = "";
    positions = std::move(_positions);
    element_indices = std::move(_element_indices);
    index_count = element_indices.size(); // renamed

    sendMeshToGPU();
}


Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}