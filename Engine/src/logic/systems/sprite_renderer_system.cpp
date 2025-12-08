#include "sprite_renderer_system.h"
#include <iostream>
#include <logic/scene_manager.h>
#include <engine_variables.h>

void SpriteRenderer_S::update() {
    if (!SystemManager::current_scene) return;
    SystemManager::current_scene->forEach<Transform_C, MeshRenderer_C>(
    [this](Entity e, Transform_C& transform_c, MeshRenderer_C& mesh_c) {
        draw(transform_c, mesh_c);
    });
}

SpriteRenderer_S::SpriteRenderer_S(std::string _shader_name, Asset _vertex, Asset _fragment) : 
                                shader_name(_shader_name), vertex_asset(_vertex), fragment_asset(_fragment) { }

glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

void SpriteRenderer_S::awake() {
    ShaderManager::load(shader_name, vertex_asset, fragment_asset);
    glUseProgram(ShaderManager::get(shader_name).gl_id);
}

void SpriteRenderer_S::draw(Transform_C& t, MeshRenderer_C& c)
{
    if (!c.mesh) {
        std::cout << "[WARN] draw: mesh pointer null\n";
        return;
    }
    if (c.mesh->VAO == 0 || c.mesh->index_count == 0) return;

    Shader& shader = ShaderManager::get(shader_name);
    glUseProgram(shader.gl_id);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("model", t.worldMatrix);
    c.material.texture.uploadIfNeeded();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, c.material.texture.gpu_texture);
    int loc = glGetUniformLocation(shader.gl_id, "ourTexture");
    if (loc != -1) glUniform1i(loc, 0);
    glBindVertexArray(c.mesh->VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(c.mesh->index_count), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}