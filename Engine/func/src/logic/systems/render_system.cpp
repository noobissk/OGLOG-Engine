#include "render_system.h"

void Renderer_S::update() {
    if (!SystemManager::current_scene) return;
    SystemManager::current_scene->forEach<MeshRenderer_C>([this](Entity e, MeshRenderer_C mesh_c) {
        if (mesh_c.is_enabled)
            draw(mesh_c.mesh);
    });
}




void Renderer_S::draw(Mesh mesh_c)
{
    
}