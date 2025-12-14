#pragma once
#include "material.h"
#include <util/texture.h>

class MaterialFont : public Material {
public:
    Texture texture;
    MaterialFont(Shader* _shader, Texture _texture);
    MaterialFont(Shader* _shader, Asset _texture);
    void use() override;
    ~MaterialFont();
};