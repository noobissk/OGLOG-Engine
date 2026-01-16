#include "material_font.h"

#include <config_render.h>


MaterialFont::MaterialFont(Shader* _shader, Asset texture_asset) : Material(_shader), texture(AssetManager::assetToPath(texture_asset)) { 
    if (!glfwGetCurrentContext()) {
        std::cerr << "[FATAL] uploadIfNeeded called with NO OpenGL context\n";
        return;
    }
    texture.uploadIfNeeded(STBI_grey);
}
MaterialFont::MaterialFont(Shader* _shader, Texture _texture) : Material(_shader), texture(std::move(_texture)) { 
    if (!glfwGetCurrentContext()) {
        std::cerr << "[FATAL] uploadIfNeeded called with NO OpenGL context\n";
        return;
    }
    texture.uploadIfNeeded(STBI_grey);
}


void MaterialFont::use() {
    glUseProgram(shader->gl_id);
    int loc = glGetUniformLocation(shader->gl_id, "sdfAtlas");
    if (loc != -1) {
        glUniform1i(loc, 0);
    }
    else return;
    
    texture.uploadIfNeeded(STBI_grey);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.gpu_texture);
}

MaterialFont::~MaterialFont() = default;