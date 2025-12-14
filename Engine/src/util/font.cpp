#include "font.h"
#include <fstream>
#include <utf8.h>

Font::Font(Asset _atlas_data, Texture* _atlas) : atlas(_atlas) {
    LoadedGlyphMappings mappings = loadFileGlyphMappings(AssetManager::assetToPath(_atlas_data).string());
    resolution_per_glyph = mappings.font_res;
    for (size_t i = 0; i < mappings.mappings.size(); i++) {
        atlas_data.emplace(mappings.mappings[i].unicode, mappings.mappings[i].glyph_id);
    }
}



int Font::charToGlyphID(char c) {
    return atlas_data[c];
}


struct sdfm_header {
    uint32_t magic;        // to identify file type
    uint32_t version;      // for future compatibility
    uint32_t font_res;     // font resolution
    uint32_t glyph_count;  // number of mappings
};
LoadedGlyphMappings Font::loadFileGlyphMappings(const std::string& path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open file for reading: " + path);
    }

    sdfm_header header{};
    in.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.magic != 0x4D464453) {
        throw std::runtime_error("Invalid glyph mapping file");
    }

    if (header.version != 1) {
        throw std::runtime_error("Unsupported glyph mapping version");
    }

    LoadedGlyphMappings result;
    result.font_res = header.font_res;
    result.mappings.resize(header.glyph_count);

    for (auto& m : result.mappings) {
        in.read(reinterpret_cast<char*>(&m.unicode), sizeof(m.unicode));
        in.read(reinterpret_cast<char*>(&m.glyph_id), sizeof(m.glyph_id));
    }

    return result;
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
