#include <config_render.h>
#include "render_system.h"
#include <iostream>
#include <logic/scene_manager.h>

void Renderer_S::update() {
    if (!SystemManager::current_scene) return;
    SystemManager::current_scene->forEach<MeshRenderer_C>([this](Entity e, MeshRenderer_C& mesh_c) {
        if (mesh_c.is_enabled)
            draw(mesh_c);
    });
}


void Renderer_S::awake() {
    std::cout << "[LOG] Render system init finished." << std::endl;
}

void Renderer_S::draw(const MeshRenderer_C& c)
{
    if (!c.mesh) {
        std::cout << "[WARN] draw: mesh pointer null\n";
        return;
    }
    if (c.mesh->VAO == 0 || c.mesh->index_count == 0) return;
    glBindVertexArray(c.mesh->VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(c.mesh->index_count), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}