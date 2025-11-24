#pragma once
#include <string>
#include <config_render.h>

class Shader {
public:
    unsigned int ID; // shader program ID

    void use() const {
        glUseProgram(ID);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const {
        // 1. get the uniform location
        int location = glGetUniformLocation(ID, name.c_str());

        // 2. send the matrix to GPU
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
        // GL_FALSE because glm is column-major like OpenGL
    }
};