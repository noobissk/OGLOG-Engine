#include "mesh.h"
#include <config_render.h>
#include <iostream>

Mesh::Mesh(const std::string& s)
{
    file_path = s;
    positions = {
        -1.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f, -0.0f,
         1.0f,  1.0f,  0.0f,
    };
    elementIndices = {
        0, 1, 2,
        0, 2, 3
    };

    index_count = elementIndices.size(); // renamed

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), elementIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glBindVertexArray(0);
}


Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}