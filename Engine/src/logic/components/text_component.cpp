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

    // Get the UI element bounding box size
    UIElement_C* ui = nullptr;
    if (SystemManager::current_scene->hasComponent<UIElement_C>(entity)) {
        ui = &SystemManager::current_scene->getComponent<UIElement_C>(entity);
    }

    // Base font scale from font metrics
    float baseScale = font->font_data.font_res / font->font_data.unitsPerEm * 0.005f;
    
    // Scale cell size based on text_size (font scale factor)
    // text_size of 100 is the base, so 200 is 2x larger, 50 is 0.5x smaller
    float fontScaleFactor = text_size;
    float cell = 150.0f * baseScale * fontScaleFactor;
    float bearingOffset = 20.0f * baseScale * fontScaleFactor;
    float line_spacing_height = cell * line_spacing;
    float charWidth = character_spacing * baseScale * fontScaleFactor;
    
    int id = 0;

    // Word wrapping: split text into words and wrap based on bounding box width
    std::string wrappedText = text;
    if (ui) {
        float maxLineWidth = ui->size.x / fontScaleFactor * 0.005f;  // Scale to account for font size
        std::istringstream wordStream(text);
        std::string word;
        std::string wrappedLine;
        wrappedText.clear();
        
        while (wordStream >> word) {
            float wordWidth = word.length() * charWidth;
            float currentLineWidth = wrappedLine.length() * charWidth;
            
            // Check if adding this word would exceed the width
            if (currentLineWidth > 0 && currentLineWidth + charWidth + wordWidth > maxLineWidth) {
                // Start a new line
                wrappedText += wrappedLine + "\n";
                wrappedLine = word;
            } else {
                if (!wrappedLine.empty()) {
                    wrappedLine += " ";
                }
                wrappedLine += word;
            }
        }
        if (!wrappedLine.empty()) {
            wrappedText += wrappedLine;
        }
    }

    // Split text into lines
    std::vector<std::string> lines;
    std::stringstream ss(wrappedText);
    std::string line;
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    // Calculate line widths for alignment
    float line_width;
    

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
        float line_width = lines[line_idx].size() * character_spacing * baseScale * fontScaleFactor;
        
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

                penX += character_spacing * baseScale * fontScaleFactor;
                // penX += g.advance * font->font_data.unitsPerEm * 0.05f * scale;
                id++;
            } else {
                penX += character_spacing * baseScale * fontScaleFactor;
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

