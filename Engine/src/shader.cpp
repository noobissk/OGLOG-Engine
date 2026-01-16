#include "shader.h"
#include "service/asset_manager.h"
#include <fstream>
#include <sstream>
// === config render include ===
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
// ===   ===   =====   ===   ===

Shader::Shader() : name(""), vertex_asset(0), fragment_asset(0), gl_id(0) {}
Shader::Shader(std::string _name, int _vertex_asset, int _fragment_asset) : name(_name), vertex_asset(_vertex_asset), fragment_asset(_fragment_asset) {
    gl_id = createShader(AssetManager::assetToPath(_vertex_asset).string(), AssetManager::assetToPath(_fragment_asset).string());
}
Shader::~Shader() {
    glDeleteProgram(gl_id);
}

unsigned int Shader::createModule(const std::string& filepath, unsigned int module_type) {
    std::ifstream file;
    std::stringstream buffered_lines;
    std::string line;

    std::cout << "[LOG] Reading shader -> " << filepath << '\n';

    file.open(filepath);
    while (std::getline(file, line)) {
        buffered_lines << line << '\n';
    }
    std::string shader_source = buffered_lines.str();
    const char* shader_src = shader_source.c_str();

    buffered_lines.str("");
    file.close();

    unsigned int shader_module = glCreateShader(module_type);
    glShaderSource(shader_module, 1, &shader_src, NULL);
    glCompileShader(shader_module);


    int success;
    glGetShaderiv(shader_module, GL_COMPILE_STATUS, &success);
    if (!success) {
        char error_log[1024];
        glGetShaderInfoLog(shader_module, 1024, NULL, error_log);
        std::cout << "[ERROR] Shader module compilation failed -> " << error_log << std::endl;
    }

    return shader_module;
}


unsigned int Shader::createShader(const std::string& vertex_filepath, const std::string& fragment_filepath) {
    std::vector<unsigned int> modules;

    std::cout << "[LOG] Creating shader modules." << std::endl;
    
    modules.push_back(createModule(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(createModule(fragment_filepath, GL_FRAGMENT_SHADER));

    std::cout << "[LOG] Shader module creation successful." << std::endl;

    unsigned int shader = glCreateProgram();
    for (unsigned int shader_module : modules)
    {
        glAttachShader(shader, shader_module);
    }
    
    glLinkProgram(shader);
    
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        char error_log[1024];
        glGetProgramInfoLog(shader, 1024, NULL, error_log);
        std::cout << "[ERROR] Shader linking failed -> " << error_log << std::endl;
    }
    
    for (unsigned int module : modules)
    {
        glDeleteShader(module);
    }
    
    std::cout << "[LOG] Shader linking successful." << std::endl;
    return shader;
}

void Shader::use() const {
    glUseProgram(gl_id);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    int location = glGetUniformLocation(gl_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::setFloat(const std::string& name, float val) const {
    int location = glGetUniformLocation(gl_id, name.c_str());
    glUniform1f(location, val);
}