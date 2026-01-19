#include "text_component.h"
#include <logic/system_manager.h>
#include <iostream>
#include <sstream>

void Text_C::rebuildMesh()
{
    if (!font) {
        std::cerr << "[ERROR] rebuildMesh called with no font\n";
        return;
    }

    if (!SystemManager::current_scene || !SystemManager::current_scene->hasComponent<MeshRenderer_C>(entity)) {
        std::cerr << "[ERROR] rebuildMesh: entity has no MeshRenderer_C\n";
        return;
    }
    
    MeshRenderer_C* mesh_renderer = &SystemManager::current_scene->getComponent<MeshRenderer_C>(entity);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float scale = font->font_data.font_res / font->font_data.unitsPerEm * 0.05f;
    float cell = 150.0f * scale;
    float bearingOffset = 20.0f * scale;
    float line_spacing_height = cell * line_spacing;
    
    int id = 0;

    // Split text into lines
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    // Calculate line widths for alignment
    std::vector<float> line_widths;
    for (size_t line_idx = 0; line_idx < lines.size(); line_idx++) {
        const auto& l = lines[line_idx];
        float line_width = 0.0f;
        for (char c : l) {
            if (c != ' ') {
                int glyphID = font->charToGlyphID(c);
                if (glyphID >= 0) {
                    line_width += character_spacing * scale;
                } else {
                    // Even if glyph not found, count the space it would take
                    line_width += character_spacing * scale;
                }
            } else {
                line_width += character_spacing * scale;
            }
        }
        line_widths.push_back(line_width);
    }

    float penY = 0.0f;
    float total_height = (lines.size() - 1) * line_spacing_height;
    
    if (v_alignment == VerticalAlign::TOP) {
        penY = total_height * 0.5f;
    } else if (v_alignment == VerticalAlign::CENTER) {
        penY = 0.0f;
    } else if (v_alignment == VerticalAlign::BOTTOM) {
        penY = -total_height * 0.5f;
    }

    for (size_t line_idx = 0; line_idx < lines.size(); line_idx++) {
        const std::string& current_line = lines[line_idx];
        float line_width = line_widths[line_idx];
        
        float penX = 0.0f;
        if (alignment == TextAlign::CENTER) {
            penX = -line_width * 0.5f;
        } else if (alignment == TextAlign::RIGHT) {
            penX = -line_width;
        }

        // Process each character in the line
        for (int i = 0; i < current_line.length(); i++) {
            if (current_line[i] != ' ') {
                int glyphID = font->charToGlyphID(current_line[i]);
                if (glyphID < 0) {
                    std::cout << "[WARN] Character '" << current_line[i] << "' not found in font\n";
                    continue;
                }
                
                if (glyphID >= static_cast<int>(font->font_data.glyphs.size())) {
                    std::cout << "[ERROR] glyphID " << glyphID << " out of bounds (glyph count: " << font->font_data.glyphs.size() << ")\n";
                    continue;
                }
                
                const glyphRecord& g = font->font_data.glyphs[glyphID];
                GlyphUV uv = font->glyphIDToUV(glyphID);

                float centerY = penY;
                
                float x0 = penX + bearingOffset;
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

                penX += character_spacing * scale;
                // penX += g.advance * font->font_data.unitsPerEm * 0.05f * scale;
                id++;
            } else {
                penX += character_spacing * scale;
            }
        }

        penY -= line_spacing_height;
    }
    
    if (!mesh_renderer) {
        std::cerr << "[ERROR] rebuildMesh: mesh_renderer is null\n";
        return;
    }
    if (!mesh_renderer->material) {
        std::cerr << "[ERROR] rebuildMesh: mesh_renderer->material is null\n";
        return;
    }
    if (!mesh_renderer->material->shader) {
        std::cerr << "[ERROR] rebuildMesh: mesh_renderer->material->shader is null\n";
        return;
    }
    if (!mesh_renderer->mesh) {
        std::cerr << "[ERROR] rebuildMesh: mesh_renderer->mesh is null\n";
        return;
    }
    
    mesh_renderer->material->shader->setFloat("smoothing", 0.02f);
    mesh_renderer->mesh->updateMesh(vertices, indices);
    
    text_old = text;
    alignment_old = alignment;
    v_alignment_old = v_alignment;
}

