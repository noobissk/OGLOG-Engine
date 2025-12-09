#include "shader_manager.h"

std::unordered_map<std::string, std::unique_ptr<Shader>> ShaderManager::shaders;

Shader* ShaderManager::get(std::string name)  {
    return shaders[name].get();
}

Shader* ShaderManager::load(std::string name, int vertex, int fragment) {
    shaders[name] = std::make_unique<Shader>(name, vertex, fragment);
    return shaders[name].get();
}