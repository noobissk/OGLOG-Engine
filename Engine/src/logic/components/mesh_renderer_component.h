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

    MeshRenderer_C(std::shared_ptr<Material> _material, const std::string& _mesh_path = "") : is_enabled(true), VBO(0) {
        material = std::move(_material);
        if (_mesh_path.empty()) {
            if (auto mat = std::dynamic_pointer_cast<MaterialDefault>(material)) {
                mesh = std::make_shared<Mesh>("");
            } else if (std::dynamic_pointer_cast<MaterialFont>(material)) {
                // For text rendering, create an empty mesh that will be populated by Text_C
                std::vector<float> empty_positions;
                std::vector<unsigned int> empty_indices;
                mesh = std::make_shared<Mesh>(empty_positions, empty_indices);
            } else {
                mesh = std::make_shared<Mesh>(_mesh_path);
            }
        } else {
            mesh = std::make_shared<Mesh>(_mesh_path);
        }

    }
};