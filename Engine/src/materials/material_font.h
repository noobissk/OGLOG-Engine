#pragma once
#include "material.h"
#include <util/texture.h>

class MaterialFont : public Material {
public:
    Texture texture;
    MaterialFont(Shader* _shader, Asset sdf_atlas);
    void use() override;
    ~MaterialFont();
};