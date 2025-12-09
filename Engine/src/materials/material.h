
#pragma once
#include <string>
#include "service/asset_manager.h"
#include "shader.h"

using Asset = uint16_t;

class Material {
public:
    Shader* shader;
    Material(Shader* _shader);
    virtual void use();
    virtual ~Material();
};