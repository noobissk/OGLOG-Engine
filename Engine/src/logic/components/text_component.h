#pragma once
#include <logic/scene_manager.h>
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include "util/mesh.h"
#include "util/font.h"
#include "materials/material_font.h"


struct Text_C {
private:
    std::string text_old;
    float smoothness;
public:
    std::string text;
    int text_size;
    std::shared_ptr<Font> font;
    MeshRenderer_C* mesh_renderer;
    Text_C(MeshRenderer_C* _mesh_renderer, std::shared_ptr<Font> _font) : mesh_renderer(_mesh_renderer), font(_font), text_size(24), smoothness(0.0025f/text_size) { rebuildMesh(); }
    Text_C(MeshRenderer_C* _mesh_renderer, std::shared_ptr<Font> _font, const std::string& _text) : text(_text), text_old(_text), mesh_renderer(_mesh_renderer), font(_font), text_size(24), smoothness(0.0025f/text_size) { rebuildMesh(); }
    GlyphUV glyphIDToUV(int glyph_id);

    bool checkDirty() {
        return text != text_old;
    }

    void rebuildMesh()
    {
        text_old = text;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float penX = 0.0f;
        float penY = 0.0f;

        float scale = font->font_data.font_res / font->font_data.unitsPerEm * 0.05f;
        int id = 0;

        float baselineY = 0.0f;

        for (int i = 0; i < text.length(); i++)
        {
            if (text[i] != ' ')
            {
                int glyphID = font->charToGlyphID(text[i]);
                const glyphRecord& g = font->font_data.glyphs[glyphID];
                GlyphUV uv = font->glyphIDToUV(glyphID);

                float centerY = 0.0f;
                float cell = 150.0f * scale;

                float x0 = penX;
                float x1 = x0 + cell;

                float y0 = centerY - cell * 0.5f;
                float y1 = centerY + cell * 0.5f;

                unsigned int base = id * 4;

                vertices.insert(vertices.end(), {
                    x0, y1, 0.0f, uv.u0, uv.v0,
                    x0, y0, 0.0f, uv.u0, uv.v1,
                    x1, y0, 0.0f, uv.u1, uv.v1,
                    x1, y1, 0.0f, uv.u1, uv.v0
                });

                indices.insert(indices.end(), {
                    base + 0, base + 1, base + 2,
                    base + 0, base + 2, base + 3
                });

                penX += g.advance * scale;
                id++;
            }
            else
            {
                penX += 50.0f * scale;
            }
        }

        mesh_renderer->material->shader->setFloat("smoothing", 0.02f);
        mesh_renderer->mesh->updateMesh(vertices, indices);
    }


};