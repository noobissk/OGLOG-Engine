#include "material_font.h"
#include <config_render.h>

MaterialFont::MaterialFont(Shader* _shader, std::shared_ptr<Font> _font) : Material(_shader), font(std::move(_font)) { 
}

MaterialFont::MaterialFont(Shader* _shader, Asset _font_data, const std::vector<Asset>& _atlas_assets) : Material(_shader) {
    font = std::make_shared<Font>(_font_data, _atlas_assets);
    
    if (!glfwGetCurrentContext()) {
        std::cerr << "[FATAL] MaterialFont constructor called with NO OpenGL context\n";
        return;
    }
    
    // Upload all atlas textures
    for (Texture* atlas : font->atlases) {
        if (atlas) {
            atlas->uploadIfNeeded(STBI_grey);
        }
    }
}

void MaterialFont::use() {
    glUseProgram(shader->gl_id);
    int loc = glGetUniformLocation(shader->gl_id, "sdfAtlas");
    if (loc != -1) {
        glUniform1i(loc, 0);
    }
}

void MaterialFont::useForGlyph(uint8_t atlasIndex) {
    glUseProgram(shader->gl_id);
    
    Texture* atlas = font->getAtlas(atlasIndex);
    if (!atlas) {
        std::cerr << "[WARN] useForGlyph: atlas at index " << static_cast<int>(atlasIndex) << " not found\n";
        return;
    }
    
    int loc = glGetUniformLocation(shader->gl_id, "sdfAtlas");
    if (loc != -1) {
        glUniform1i(loc, 0);
    }
    
    atlas->uploadIfNeeded(STBI_grey);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas->gpu_texture);
}

MaterialFont::~MaterialFont() = default;