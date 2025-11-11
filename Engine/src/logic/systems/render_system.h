#pragma once
#include <config_system.h>

class Renderer_S : public System {
public:
    void update() override;
    void awake() override;
    void draw(const MeshRenderer_C& c);
};