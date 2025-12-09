#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "shader.h"

class ShaderManager {
public:
    static std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;

    static Shader* load(std::string name, int vertex, int fragment);
    static Shader* get(std::string name);
};