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
Entity e4;

void SystemManager::initialize()
{
    GLint maxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    std::cout << "Max texture size: " << maxTextureSize << " x "
              << maxTextureSize << std::endl;
    ShaderManager::load("sprite", 11, 7);
    ShaderManager::load("font", 10, 4);
    // e1 = current_scene->createEntity();
    // e2 = current_scene->createEntity();
    e3 = current_scene->createEntity();
    e4 = current_scene->createEntity();

    // current_scene->addComponent<MeshRenderer_C>(e1, MeshRenderer_C(std::make_shared<MaterialDefault>(ShaderManager::get("sprite"), 17)));
    // current_scene->addComponent<Transform_C>(e1, Transform_C(e1));

    // current_scene->addComponent<MeshRenderer_C>(e2, MeshRenderer_C(std::make_shared<MaterialDefault>(ShaderManager::get("sprite"), 13)));
    // current_scene->addComponent<Transform_C>(e2, Transform_C(e2));
    
    auto* c = &current_scene->addComponent<MeshRenderer_C>(e3, MeshRenderer_C(std::make_shared<MaterialFont>(ShaderManager::get("font"), 18)));
    current_scene->addComponent<Transform_C>(e3, Transform_C(e3));
    auto* text_c = &current_scene->addComponent<Text_C>(e3, Text_C(c, std::make_shared<Font>(15, &std::dynamic_pointer_cast<MaterialFont>(c->material)->texture) ));

    Transform_C& t = current_scene->getComponent<Transform_C>(e3);
    t.position.x = - 1;

    // auto* c1 = &current_scene->addComponent<MeshRenderer_C>(e4, MeshRenderer_C(std::make_shared<MaterialFont>(ShaderManager::get("font"), 21)));
    // current_scene->addComponent<Transform_C>(e4, Transform_C(e4));
    // auto* text_c1 = &current_scene->addComponent<Text_C>(e4, Text_C(c1, std::make_shared<Font>(14, &std::dynamic_pointer_cast<MaterialFont>(c1->material)->texture) ));

    // text_c1->text = "Dobry den, gazda Juraj! test";
    
    // Transform_C& t1 = current_scene->getComponent<Transform_C>(e4);
    // t1.position.x = - 0.8;
    // t1.position.y = - 0.3;
}

void SystemManager::update()
{
    if (!systems.empty()) systems[0]->update();
    
    // Transform_C* transform1 = &current_scene->getComponent<Transform_C>(e3);
    // Transform_C* transform2 = &current_scene->getComponent<Transform_C>(e2);

    // transform2->position.x = sin(-Time::time * 2.0);
    // transform2->position.z = cos(-Time::time * 2.0);

    // transform1->scale.x = (sin(Time::time) + 1.5) * 0.25f;
    // transform1->scale.y = (sin(Time::time) + 1.5) * 0.25f;

    // transform1->rotation.z = static_cast<float>(-Time::time);
    Text_C& t = current_scene->getComponent<Text_C>(e3);
    std::string txt = "Good day mister Juraj. Ť";
    std::string result = txt.substr(0, std::round(((Time::sine + 1) * 0.5f) * txt.length()));
    t.text = result;
  
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

