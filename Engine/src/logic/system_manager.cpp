#include <logic/system_manager.h>
#include <logic/scene_manager.h>
#include <config_system.h>
#include <logic/components/transform_component.h>
#include <logic/components/mesh_renderer_component.h>
#include <iostream>
#include <engine_variables.h>
#include <util/time.h>

std::vector<System*> SystemManager::systems = {
    new Time(),
    new Transform_S(),
    new Renderer_S()
};

Scene* SystemManager::current_scene = nullptr;

Entity e1;
Entity e2;

void SystemManager::initialize()
{
    unsigned int current_shader = shader.ID;

    // load systems to memory
    e1 = current_scene->createEntity();
    e2 = current_scene->createEntity();

    // current_scene->addComponent<Transform_C>(e, Transform_C{});
    current_scene->addComponent<MeshRenderer_C>(e1, MeshRenderer_C{""});
    auto &transform1 = current_scene->addComponent<Transform_C>(e1, Transform_C{});
    
    current_scene->addComponent<MeshRenderer_C>(e2, MeshRenderer_C{""});
    auto &transform2 = current_scene->addComponent<Transform_C>(e2, Transform_C{});
    
    transform2.position.y = 1.0f;
    transform2.scale = glm::vec3(0.5f, 0.5f, 0.5f);
}

void SystemManager::update()
{
    // Update Time first so Time::time is current
    if (!systems.empty()) systems[0]->update();
    
    Transform_C* transform1 = &current_scene->addComponent<Transform_C>(e1, Transform_C{});
    Transform_C* transform2 = &current_scene->addComponent<Transform_C>(e2, Transform_C{});

    transform1->position.x = sin(-Time::time * 2.0);
    transform1->position.y = cos(-Time::time * 2.0);

    transform2->position.x = sin(Time::time * 4.0);
    transform2->position.y = cos(Time::time * 4.0);

    transform2->scale.x = (sin(Time::time) + 1.5) * 0.25f;
    transform2->scale.y = (sin(Time::time) + 1.5) * 0.25f;
    
    transform2->rotation.z += Time::delta;
    transform1->rotation.z = static_cast<float>(-Time::time);

    // Update remaining systems (Transform, Renderer, ...)
    for (int i = 1; i < static_cast<int>(systems.size()); ++i) {
        systems[i]->update();
    }
}


void SystemManager::awake()
{
    for (System* sys : systems) {
        sys->awake();
    }
}

void SystemManager::start()
{
    
}

