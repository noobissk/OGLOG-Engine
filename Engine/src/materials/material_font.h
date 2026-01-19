#pragma once
#include "material.h"
#include <util/texture.h>
#include <util/font.h>

class MaterialFont : public Material {
public:
    std::shared_ptr<Font> font;
    MaterialFont(Shader* _shader, std::shared_ptr<Font> _font);
    MaterialFont(Shader* _shader, Asset _font_data, const std::vector<Asset>& _atlas_assets);
    void use() override;
    void useForGlyph(uint8_t atlasIndex);
    ~MaterialFont();
};