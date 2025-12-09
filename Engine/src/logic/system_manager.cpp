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
    new SpriteRenderer_S()
};

Scene* SystemManager::current_scene = nullptr;

Entity e1;
Entity e2;
Entity e3;

void SystemManager::initialize()
{
    ShaderManager::load("sprite", 9, 4);
    ShaderManager::load("font", 11, 10);
    e1 = current_scene->createEntity();
    e2 = current_scene->createEntity();
    // e3 = current_scene->createEntity();

    current_scene->addComponent<MeshRenderer_C>(e1, MeshRenderer_C("", std::make_shared<MaterialDefault>(ShaderManager::get("sprite"), 12)));
    current_scene->addComponent<Transform_C>(e1, Transform_C(e1));

    auto& meshR = current_scene->addComponent<MeshRenderer_C>(e2, MeshRenderer_C("", std::make_shared<MaterialDefault>(ShaderManager::get("sprite"), 14)));
    current_scene->addComponent<Transform_C>(e2, Transform_C(e2));

    // current_scene->addComponent<MeshRenderer_C>(e3, MeshRenderer_C("", MaterialFont(ShaderManager::get("font").gl_id, 17)));
    // current_scene->addComponent<Transform_C>(e3, Transform_C{e3});

    Transform_C& t2 = current_scene->getComponent<Transform_C>(e2);
    
    Transform_C& t1 = current_scene->getComponent<Transform_C>(e1);

    t2.parent = t1.self;

    auto mat = std::dynamic_pointer_cast<MaterialDefault>(meshR.material);
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

