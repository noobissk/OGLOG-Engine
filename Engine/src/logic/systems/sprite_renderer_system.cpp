#include "sprite_renderer_system.h"
#include <iostream>
#include <logic/scene_manager.h>
#include <engine_variables.h>
#include "materials/material_default.h"

void SpriteRenderer_S::update() {
    if (!SystemManager::current_scene) return;
    SystemManager::current_scene->forEach<Transform_C, MeshRenderer_C>(
    [this](Entity e, Transform_C& transform_c, MeshRenderer_C& mesh_c) {
        draw(transform_c, mesh_c);
    });
}

SpriteRenderer_S::SpriteRenderer_S() { }

glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

void SpriteRenderer_S::awake() { }

void SpriteRenderer_S::draw(Transform_C& t, MeshRenderer_C& c)
{
    if (!c.mesh) {
        std::cout << "[WARN] draw: mesh pointer null\n";
        return;
    }
    if (c.material->shader->vertex_asset == 0) {
        std::cout << "[WARN] draw: material is null\n";
        return;
    }
    if (c.mesh->VAO == 0 || c.mesh->index_count == 0) return;

    if (auto mat = std::dynamic_pointer_cast<MaterialDefault>(c.material)) {
        glUseProgram(mat->shader->gl_id);

        mat->shader->setMat4("view", view);
        mat->shader->setMat4("projection", projection);
        mat->shader->setMat4("model", t.worldMatrix);

        mat->use();
        glBindVertexArray(c.mesh->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(c.mesh->index_count), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    else if (auto mat = std::dynamic_pointer_cast<MaterialFont>(c.material)) {
        std::cout << "test render" << std::endl;
    }
}