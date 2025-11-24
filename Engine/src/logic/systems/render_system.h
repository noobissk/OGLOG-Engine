#pragma once
#include <config_system.h>
#include <config_component.h>
#include <config_render.h>

class Renderer_S : public System {
public:
    void update() override;
    void awake() override;
    void draw(Transform_C& t, MeshRenderer_C& c);
};