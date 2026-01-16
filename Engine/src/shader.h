#pragma once
#include <string>
#include <glm_math.h>



class Shader {
public:
    std::string name;
    int vertex_asset;
    int fragment_asset;

    int gl_id;

    Shader();
    Shader(std::string _name, int _vertexAsset, int _fragmentAsset);
    ~Shader();
    static unsigned int createModule(const std::string& filepath, unsigned int module_type);
    static unsigned int createShader(const std::string& vertex_module, const std::string& fragment_module);

    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setFloat(const std::string& name, float val) const;
};