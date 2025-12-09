#pragma once
#include <util/mesh.h>
#include <string>
#include <materials/material_default.h>
#include <materials/material_font.h>
#include <memory>


struct MeshRenderer_C {
public:
    bool is_enabled;
    unsigned int VBO;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;

    MeshRenderer_C(const std::string& s, std::shared_ptr<Material> mat) : is_enabled(true), VBO(0), mesh(std::make_shared<Mesh>(s)), material(mat) { }
};