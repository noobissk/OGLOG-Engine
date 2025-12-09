#include "material_font.h"

#include <config_render.h>


MaterialFont::MaterialFont(Shader* _shader, Asset _texture_asset) : Material(_shader), texture(AssetManager::assetToPath(_texture_asset)) {}

void MaterialFont::use() {
    glUseProgram(shader->gl_id);
    int loc = glGetUniformLocation(shader->gl_id, "sdfAtlas");
    if (loc != -1) {
        glUniform1i(loc, 0);
    }
}

MaterialFont::~MaterialFont() = default;