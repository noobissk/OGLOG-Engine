#include "font.h"
#include <fstream>
#include <utf8.h>
#include <vector>

Font::Font(Asset _atlas_data, Texture* _atlas) : atlas(_atlas) {
    font_data = loadFileGlyphMappings(AssetManager::assetToPath(_atlas_data).string());
    resolution_per_glyph = font_data.font_res;
    for (size_t i = 0; i < font_data.glyphs.size(); i++) {
        atlas_data.emplace(font_data.glyphs[i].unicode, font_data.glyphs[i].glyph_id);
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



struct sdfm_glyph {
    uint32_t unicode;
    uint32_t glyph_id;

    float advance;
    float bearingX;
    float bearingY;
    float width;
    float height;
};
struct sdfm_header {
    uint32_t magic;
    uint32_t version;
    uint32_t font_res;
    uint32_t glyph_count;
    float unitsPerEm;
    float ascender;
    float descender;
};
FontData Font::loadFileGlyphMappings(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    sdfm_header header{};
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    
    if (header.magic != 0x4D464453) {
        throw std::runtime_error("Invalid SDF font file");
    }
    if (header.version != 1) {
        throw std::runtime_error("Unsupported SDF font version");
    }
    
    FontData font{};
    font.font_res   = header.font_res;
    font.unitsPerEm = header.unitsPerEm;
    font.ascender   = header.ascender;
    font.descender  = header.descender;

    font.glyphs.resize(header.glyph_count);


    for (uint32_t i = 0; i < header.glyph_count; i++) {
        sdfm_glyph bin{};
        in.read(reinterpret_cast<char*>(&bin), sizeof(bin));

        glyphRecord& g = font.glyphs[i];
        g.unicode  = bin.unicode;
        g.glyph_id = bin.glyph_id;
        g.advance  = bin.advance;
        g.bearingX = bin.bearingX;
        g.bearingY = bin.bearingY;
        g.width    = bin.width;
        g.height   = bin.height;
    }

    return font;
}



GlyphUV Font::glyphIDToUV(int glyph_id) const
{
    int atlasW = atlas->width;
    int atlasH = atlas->height;

    int cellsPerRow = atlasW / resolution_per_glyph;

    int col = glyph_id % cellsPerRow;
    int row = glyph_id / cellsPerRow;

    int x0 = col * resolution_per_glyph;
    int y0 = row * resolution_per_glyph;
    int x1 = x0 + resolution_per_glyph;
    int y1 = y0 + resolution_per_glyph;

    float u0 = float(x0) / atlasW;
    float v0 = float(y0) / atlasH;
    float u1 = float(x1) / atlasW;
    float v1 = float(y1) / atlasH;

    return { u0, v0, u1, v1 };
}
