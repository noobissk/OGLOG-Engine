#include "font.h"
#include <fstream>
#include <utf8.h>
#include <vector>

Font::Font(Asset _font_data, const std::vector<Asset>& _atlas_assets) {
    font_data = loadFileGlyphMappings(AssetManager::assetToPath(_font_data).string());
    resolution_per_glyph = font_data.font_res;
    
    for (Asset atlas_asset : _atlas_assets) {
        atlases.push_back(new Texture(AssetManager::assetToPath(atlas_asset)));
    }
    
    // Map unicode → vector index (not glyph_id, since glyphs are stored sequentially in the file)
    for (size_t i = 0; i < font_data.glyphs.size(); i++) {
        atlas_data.emplace(font_data.glyphs[i].unicode, i);
    }
}

Font::Font(Asset _font_data, Texture* _atlas) {
    font_data = loadFileGlyphMappings(AssetManager::assetToPath(_font_data).string());
    resolution_per_glyph = font_data.font_res;
    atlases.push_back(_atlas);
    // Map unicode → vector index (not glyph_id, since glyphs are stored sequentially in the file)
    for (size_t i = 0; i < font_data.glyphs.size(); i++) {
        atlas_data.emplace(font_data.glyphs[i].unicode, i);
    }
}



int Font::charToGlyphID(char c) {
    auto c_unicode = static_cast<unsigned int>(static_cast<unsigned char>(c));
    auto it = atlas_data.find(c_unicode);
    if (it == atlas_data.end()) {
        return -1;
    }
    return it->second;
}



FontData Font::loadFileGlyphMappings(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    uint32_t glyphCount = 0;
    in.read(reinterpret_cast<char*>(&glyphCount), sizeof(uint32_t));

    float ascender = 0.0f, descender = 0.0f, unitsPerEm = 0.0f;
    in.read(reinterpret_cast<char*>(&ascender), sizeof(float));
    in.read(reinterpret_cast<char*>(&descender), sizeof(float));
    in.read(reinterpret_cast<char*>(&unitsPerEm), sizeof(float));
    
    FontData font{};
    font.unitsPerEm = unitsPerEm;
    font.ascender   = ascender;
    font.descender  = descender;
    font.font_res   = 64; // This is set based on your atlas

    font.glyphs.resize(glyphCount);

    for (uint32_t i = 0; i < glyphCount; i++) {
        glyphRecord& g = font.glyphs[i];
        
        in.read(reinterpret_cast<char*>(&g.unicode), sizeof(uint32_t));
        in.read(reinterpret_cast<char*>(&g.glyph_id), sizeof(uint32_t));

        in.read(reinterpret_cast<char*>(&g.advance), sizeof(float));
        in.read(reinterpret_cast<char*>(&g.bearingX), sizeof(float));
        in.read(reinterpret_cast<char*>(&g.bearingY), sizeof(float));
        in.read(reinterpret_cast<char*>(&g.width), sizeof(float));
        in.read(reinterpret_cast<char*>(&g.height), sizeof(float));

        in.read(reinterpret_cast<char*>(&g.atlasIndex), sizeof(uint8_t));

        in.read(reinterpret_cast<char*>(&g.u0), sizeof(float));
        in.read(reinterpret_cast<char*>(&g.v0), sizeof(float));
        in.read(reinterpret_cast<char*>(&g.u1), sizeof(float));
        in.read(reinterpret_cast<char*>(&g.v1), sizeof(float));
        
        if (i == 0 || i == 141) { // Log first glyph and glyph 141 (probably 'G')
            std::cout << "[LOG] Loaded glyph " << i << ": unicode=" << g.unicode << ", atlasIdx=" << (int)g.atlasIndex 
                      << ", uv=(" << g.u0 << "," << g.v0 << ")-(" << g.u1 << "," << g.v1 << ")\n";
        }
    }

    return font;
}



GlyphUV Font::glyphIDToUV(int glyph_id) const
{
    if (glyph_id < 0 || glyph_id >= static_cast<int>(font_data.glyphs.size())) {
        return { 0.0f, 0.0f, 0.0f, 0.0f };
    }

    const glyphRecord& g = font_data.glyphs[glyph_id];
    return { g.u0, g.v0, g.u1, g.v1 };
}

Texture* Font::getAtlas(uint8_t index) const
{
    if (index >= atlases.size()) {
        return nullptr;
    }
    return atlases[index];
}
