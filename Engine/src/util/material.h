
#pragma once
#include <string>
#include <service/asset_manager.h>
#include <util/texture.h>
#include <engine_variables.h>
using Asset = uint16_t;

class Material {
public:
    Texture texture;

    Material(Asset texture_asset);
    // void applyTexture(int texture);
    void use(int shader);
    ~Material();
};