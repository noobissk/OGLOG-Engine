#pragma once
#include <logic/systems/systems.h>

class Renderer_S : public System {
public:
    void update() override;
    void draw(Mesh mesh);
};