#pragma once
#include <logic/scene_manager.h>
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <iostream>
#include "util/mesh.h"
#include "util/font.h"
#include "materials/material_font.h"

// Forward declaration
class SystemManager;

// Text alignment options
enum class TextAlign {
    LEFT = 0,
    CENTER = 1,
    RIGHT = 2
};

enum class VerticalAlign {
    TOP = 0,
    CENTER = 1,
    BOTTOM = 2
};

struct Text_C {
private:
    std::string text_old;
    TextAlign alignment_old;
    VerticalAlign v_alignment_old;
    float smoothness;
    Entity entity;
public:
    std::string text;
    int text_size;
    std::shared_ptr<Font> font;
    TextAlign alignment;
    VerticalAlign v_alignment;
    float character_spacing;
    float line_spacing;
    
    Text_C() : text(""), text_old(""), entity(0), font(nullptr), text_size(100), smoothness(0.0025f/24), alignment(TextAlign::LEFT), alignment_old(TextAlign::LEFT), v_alignment(VerticalAlign::TOP), v_alignment_old(VerticalAlign::TOP), character_spacing(50.0f), line_spacing(0.5f) {}
    Text_C(Entity _entity, std::shared_ptr<Font> _font) : text(""), text_old(""), entity(_entity), font(_font), text_size(100), smoothness(0.0025f/24), alignment(TextAlign::LEFT), alignment_old(TextAlign::LEFT), v_alignment(VerticalAlign::TOP), v_alignment_old(VerticalAlign::TOP), character_spacing(50.0f), line_spacing(0.5f) {}
    Text_C(Entity _entity, std::shared_ptr<Font> _font, const std::string& _text) : text(_text), text_old(_text), entity(_entity), font(_font), text_size(100), smoothness(0.0025f/24), alignment(TextAlign::LEFT), alignment_old(TextAlign::LEFT), v_alignment(VerticalAlign::TOP), v_alignment_old(VerticalAlign::TOP), character_spacing(50.0f), line_spacing(1.5f) { if (!text.empty()) rebuildMesh(); }

    bool checkDirty() {
        return text != text_old || alignment != alignment_old || v_alignment != v_alignment_old;
    }

    void rebuildMesh();
    
    // Force rebuild regardless of dirty state
    void forceRebuild() {
        text_old = "";  // Mark as dirty
        rebuildMesh();
    }


};