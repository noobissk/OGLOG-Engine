#pragma once
#include <config_system.h>
#include <config_component.h>

class Renderer_S : public System {
public:
    void update() override;
    void awake() override;
    void draw(MeshRenderer_C& c);
};