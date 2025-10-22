#pragma once
#include <util/mesh.h>
#include <string>


struct MeshRenderer_C {
public:
    bool is_enabled;
    Mesh mesh;

    MeshRenderer_C(const std::string& s) : is_enabled(true), mesh(s) {}
};