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

    void rebuildMesh() {
        text_old = text;
        
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        
        auto mat = std::dynamic_pointer_cast<MaterialFont>(mesh_renderer->material);
        int numCollumn = mat->texture.width / font->resolution_per_glyph;
        
        float mesh_offset = -((float) text.length()) / 2.0f / 8.0f;
        float mesh_offset_increase = 1.0f / 8.0f;
        
        for (int i = 0; i < text.length(); i++) {
            mesh_offset += mesh_offset_increase;
            int id = font->charToGlyphID(text[i]);
            std::cout << id << std::endl;
            auto uv = font->glyphIDToUV(id);
            vertices.insert(vertices.end(), {
                // positions                                  // texture coords
                -0.125f + mesh_offset,  0.125f,  0.0f,    uv.u0, uv.v0,
                -0.125f + mesh_offset, -0.125f,  0.0f,    uv.u0, uv.v1,
                 0.125f + mesh_offset, -0.125f,  0.0f,    uv.u1, uv.v1,
                 0.125f + mesh_offset,  0.125f,  0.0f,    uv.u1, uv.v0
            });
        
            unsigned int base = i * 4;

            indices.push_back(base + 0);
            indices.push_back(base + 1);
            indices.push_back(base + 2);
            indices.push_back(base + 0);
            indices.push_back(base + 2);
            indices.push_back(base + 3);
        }

        mesh_renderer->mesh->updateMesh(vertices, indices);
        int location = glGetUniformLocation(mesh_renderer->material->shader->gl_id, "smoothing");
        glUniform1f(location, smoothness);
    }
};