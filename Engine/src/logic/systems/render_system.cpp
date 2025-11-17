#include "render_system.h"
#include <iostream>
#include <logic/scene_manager.h>
#include <engine_variables.h>

void Renderer_S::update() {
    if (!SystemManager::current_scene) return;
    SystemManager::current_scene->forEach<MeshRenderer_C>([this](Entity e, MeshRenderer_C& mesh_c) {
        if (mesh_c.is_enabled)
            draw(mesh_c);
    });
}


void Renderer_S::awake() {
    glUseProgram(current_shader);
}

void Renderer_S::draw(MeshRenderer_C& c)
{
    if (!c.mesh) {
        std::cout << "[WARN] draw: mesh pointer null\n";
        return;
    }
    if (c.mesh->VAO == 0 || c.mesh->index_count == 0) return;
    // Ensure the texture is uploaded in this (current) GL context, then bind
    c.material.texture.uploadIfNeeded();
    glUseProgram(current_shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, c.material.texture.gpu_texture);
    int loc = glGetUniformLocation(current_shader, "ourTexture");
    if (loc != -1) glUniform1i(loc, 0);
    glBindVertexArray(c.mesh->VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(c.mesh->index_count), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}