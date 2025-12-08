#pragma once
#include <string>
#include <config_system.h>
#include <config_component.h>
#include <config_render.h>

class SpriteRenderer_S : public System {
public:
    std::string shader_name;
    Asset vertex_asset;
    Asset fragment_asset;
    
    void update() override;
    void awake() override;
    void draw(Transform_C& t, MeshRenderer_C& c);
    SpriteRenderer_S(std::string _shader_name, Asset _vertex, Asset _fragment);
};