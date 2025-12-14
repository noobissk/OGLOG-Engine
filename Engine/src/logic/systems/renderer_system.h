#pragma once
#include <string>
#include <config_system.h>
#include <config_component.h>
#include <config_render.h>

class SpriteRenderer_S : public System {
public:
    void update() override;
    void awake() override;
    void draw(Entity e, Transform_C& t, MeshRenderer_C& c);
    SpriteRenderer_S();
};