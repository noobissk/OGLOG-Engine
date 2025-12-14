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
                float aspect_position = static_cast<float>(mat->texture.width) / static_cast<float>(mat->texture.height) * 0.5f;
                std::vector<float> positions = {
                    // positions            // texture coords
                    -aspect_position,  0.5f,  0.0f,    1.0f, 0.0f,
                    -aspect_position, -0.5f,  0.0f,    1.0f, 1.0f,
                     aspect_position, -0.5f,  0.0f,    0.0f, 1.0f,
                     aspect_position,  0.5f,  0.0f,    0.0f, 0.0f,
                };
                std::vector<unsigned int> element_indices = {
                    0, 1, 2,
                    0, 2, 3
                };
                mesh = std::make_shared<Mesh>(positions, element_indices);
            } else {
                mesh = std::make_shared<Mesh>(_mesh_path);
            }
        } else {
            mesh = std::make_shared<Mesh>(_mesh_path);
        }

    }
};