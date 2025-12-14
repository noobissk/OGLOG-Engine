#pragma once
#include <string>
#include <filesystem>
#include "texture.h"
#include <unordered_map>
#include "service/asset_manager.h"


struct GlyphUV {
    float u0, v0;
    float u1, v1;
};

struct glyphMapping {
    unsigned int unicode;
    unsigned int glyph_id;
    glyphMapping() : unicode(), glyph_id() {}
    glyphMapping(unsigned int _unicode, unsigned int _glyph_id) : unicode(_unicode), glyph_id(_glyph_id) {}
};

struct LoadedGlyphMappings {
    uint32_t font_res;
    std::vector<glyphMapping> mappings;
};

class Font {
    std::unordered_map<char, int> atlas_data;
    LoadedGlyphMappings loadFileGlyphMappings(const std::string& path);
public:
    int resolution_per_glyph;
    Texture* atlas;
    Font(Asset _atlas_data, Texture* _atlas);
    int charToGlyphID(char c);
    GlyphUV glyphIDToUV(int glyph_id) const;
};