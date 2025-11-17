#include "material.h"
#include <config_render.h>


Material::Material(Asset texture_asset) : texture(AssetManager::pathToAsset(texture_asset)) {
}

void Material::use(int shader) {
    glUseProgram(shader);
    int loc = glGetUniformLocation(shader, "ourTexture");
    if (loc != -1) {
        glUniform1i(loc, 0);
    }
}

Material::~Material() = default;