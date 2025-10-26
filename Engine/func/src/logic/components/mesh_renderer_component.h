#pragma once
#include <util/mesh.h>
#include <string>
#include <util/material.h>
#include <memory>


struct MeshRenderer_C {
public:
    bool is_enabled;
    unsigned int VBO;
    std::shared_ptr<Mesh> mesh;
    Material material;

    MeshRenderer_C(const std::string& s) : is_enabled(true), mesh(std::make_shared<Mesh>(s)) {}
};