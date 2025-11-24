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

    e1 = current_scene->createEntity();
    e2 = current_scene->createEntity();

    current_scene->addComponent<MeshRenderer_C>(e1, MeshRenderer_C{""});
    current_scene->addComponent<Transform_C>(e1, Transform_C{e1});
    
    current_scene->addComponent<MeshRenderer_C>(e2, MeshRenderer_C{""});
    current_scene->addComponent<Transform_C>(e2, Transform_C{e2});

    Transform_C& t2 = current_scene->getComponent<Transform_C>(e2);
    
    Transform_C& t1 = current_scene->getComponent<Transform_C>(e1);
    
    t2.parent = t1.self;
}

void SystemManager::update()
{
    if (!systems.empty()) systems[0]->update();
    
    Transform_C* transform1 = &current_scene->getComponent<Transform_C>(e1);
    Transform_C* transform2 = &current_scene->getComponent<Transform_C>(e2);

    transform2->position.x = sin(-Time::time * 2.0);

    transform1->scale.x = (sin(Time::time) + 1.5) * 0.25f;
    transform1->scale.y = (sin(Time::time) + 1.5) * 0.25f;

    transform1->rotation.z = static_cast<float>(-Time::time);

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

