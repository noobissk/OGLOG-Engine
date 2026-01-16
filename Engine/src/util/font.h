#pragma once
#include <string>
#include <filesystem>
#include "texture.h"
#include <unordered_map>
#include "service/asset_manager.h"



struct glyphRecord {
    uint32_t unicode;
    uint32_t glyph_id;

    float advance;
    float bearingX;
    float bearingY;
    float width;
    float height;
    glyphRecord() : unicode(), glyph_id() {}
    glyphRecord(unsigned int _unicode, unsigned int _glyph_id) : unicode(_unicode), glyph_id(_glyph_id) {}
};
struct FontData {
    uint32_t font_res;
    float unitsPerEm;
    float ascender;
    float descender;
    std::vector<glyphRecord> glyphs;
};
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

class Font {
    std::unordered_map<char, int> atlas_data;
    FontData loadFileGlyphMappings(const std::string& path);
public:
    FontData font_data;
    int resolution_per_glyph;
    Texture* atlas;
    Font(Asset _atlas_data, Texture* _atlas);
    int charToGlyphID(char c);
    GlyphUV glyphIDToUV(int glyph_id) const;
};