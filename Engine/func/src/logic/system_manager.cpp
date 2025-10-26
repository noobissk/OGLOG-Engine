#include <logic/system_manager.h>
#include <logic/scene_manager.h>
#include <config_system.h>
#include <logic/components/transform_component.h>
#include <logic/components/mesh_renderer_component.h>
#include <iostream>


std::vector<System*> SystemManager::systems = {
    new Renderer_S()
    // Transform_S()
};

Scene* SystemManager::current_scene = nullptr;

void SystemManager::initialize()
{
    // load systems to memory
    Entity e = current_scene->createEntity();

    // current_scene->addComponent<Transform_C>(e, Transform_C{});
    current_scene->addComponent<MeshRenderer_C>(e, MeshRenderer_C{""});
}

void SystemManager::update()
{
    for (int i = 0; i < systems.size(); i++)
    {
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

