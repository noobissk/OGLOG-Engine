#include "renderer_system.h"
#include <iostream>
#include <logic/scene_manager.h>
#include <engine_variables.h>
#include "materials/material_default.h"

void SpriteRenderer_S::update() {
    if (!SystemManager::current_scene) return;
    int i = 0;
    SystemManager::current_scene->forEach<Transform_C, MeshRenderer_C>(
    [this](Entity e, Transform_C& transform_c, MeshRenderer_C& mesh_c) {
        draw(e, transform_c, mesh_c);
    });
}

SpriteRenderer_S::SpriteRenderer_S() { }

float aspect() {
    return static_cast<float>(resolution.x) / static_cast<float>(resolution.y);
}


glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::ortho(
    -aspect(), aspect(),            // left / right
    -1.0f, 1.0f,                    // bottom / top
    -1.0f, 1.0f                     // near / fa
);

void SpriteRenderer_S::awake() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SpriteRenderer_S::draw(Entity e, Transform_C& t, MeshRenderer_C& c)
{
    if (!c.mesh) {
        std::cout << "[WARN] draw: mesh pointer null\n";
        return;
    }
    if (c.material->shader->vertex_asset == 0) {
        std::cout << "[WARN] draw: material is null\n";
        return;
    }


    if (auto mat = std::dynamic_pointer_cast<MaterialDefault>(c.material)) {
        mat->shader->use();

        mat->shader->setMat4("model", t.worldMatrix);
        mat->shader->setMat4("view", view);
        mat->shader->setMat4("projection", projection);

        mat->use();
        glBindVertexArray(c.mesh->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(c.mesh->index_count), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        return;
    }
    if (SystemManager::current_scene->hasComponent<Text_C>(e)) {
        Text_C* text_c = &SystemManager::current_scene->getComponent<Text_C>(e);
        auto mat = std::dynamic_pointer_cast<MaterialFont>(c.material);
        
        if (!mat) {
            std::cout << "[ERROR] Text entity has no MaterialFont\n";
            return;
        }
        
        if (!text_c->font) {
            std::cout << "[ERROR] Text component has no font\n";
            return;
        }

        mat->shader->use();

        if (text_c->checkDirty())
            text_c->rebuildMesh();

        if (!c.mesh) {
            std::cout << "[WARN] Text mesh is null\n";
            return;
        }

        mat->shader->setMat4("model", t.worldMatrix);
        mat->shader->setMat4("view", view);
        mat->shader->setMat4("projection", projection);

        // Bind the correct atlas texture for the font
        if (text_c->font && text_c->font->atlases.size() > 0) {
            mat->useForGlyph(0);
        } else {
            std::cout << "[WARN] No atlases available for text rendering\n";
            mat->use();
        }
        
        glBindVertexArray(c.mesh->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(c.mesh->index_count), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}