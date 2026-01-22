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
    new Canvas_S(),
    new SpriteRenderer_S()
};

Scene* SystemManager::current_scene = nullptr;

Entity e1;
Entity e2;
Entity e3;
Entity e4;

void allingmentCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        Text_C& t1 = SystemManager::current_scene->getComponent<Text_C>(e3);
        t1.alignment = (TextAlign)((static_cast<int>(t1.alignment) + 1) % 3);
        Text_C& t2 = SystemManager::current_scene->getComponent<Text_C>(e4);
        t2.alignment = (TextAlign)((static_cast<int>(t2.alignment) - 1 + 3) % 3);
    }
}

void SystemManager::initialize()
{
    GLint maxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    std::cout << "Max texture size: " << maxTextureSize << " x "
              << maxTextureSize << std::endl;
    ShaderManager::load("sprite", 11, 7);
    ShaderManager::load("font", 10, 4);
    e1 = current_scene->createEntity();
    e2 = current_scene->createEntity();
    e3 = current_scene->createEntity();
    e4 = current_scene->createEntity();

    current_scene->addComponent<MeshRenderer_C>(e1, MeshRenderer_C(std::make_shared<MaterialDefault>(ShaderManager::get("sprite"), 17)));
    auto* ui_05 = &current_scene->addComponent<UIElement_C>(e1, UIElement_C(glm::vec2(50.0f, 50.0f), glm::vec2(100.0f, 100.0f)));
    ui_05->canvas = e2;
    current_scene->addComponent<Transform_C>(e1, Transform_C(e1));

    // current_scene->addComponent<MeshRenderer_C>(e2, MeshRenderer_C(std::make_shared<MaterialDefault>(ShaderManager::get("sprite"), 13)));
    // current_scene->addComponent<Transform_C>(e2, Transform_C(e2))
    current_scene->addComponent<Canvas_C>(e2, Canvas_C(1920.0f, 1080.0f));
    
    auto* c1 = &current_scene->addComponent<MeshRenderer_C>(e3, MeshRenderer_C(std::make_shared<MaterialFont>(ShaderManager::get("font"), 39, std::vector<Asset>{27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38})));
    auto* ui1 = &current_scene->addComponent<UIElement_C>(e3, UIElement_C(glm::vec2(0.0f, 250.0f), glm::vec2(200.0f, 200.0f)));
    ui1->canvas = e2;

    current_scene->addComponent<Transform_C>(e3, Transform_C(e3));
    
    auto mat_font1 = std::dynamic_pointer_cast<MaterialFont>(c1->material);
    if (!mat_font1) {
        std::cout << "[ERROR] Failed to cast material to MaterialFont\n";
        return;
    }
    
    if (!mat_font1->font) {
        std::cout << "[ERROR] MaterialFont has no font loaded\n";
        return;
    }
    auto* c2 = &current_scene->addComponent<MeshRenderer_C>(e4, MeshRenderer_C(std::make_shared<MaterialFont>(ShaderManager::get("font"), 20, std::vector<Asset>{14, 15, 18, 19})));
    auto* ui2 = &current_scene->addComponent<UIElement_C>(e4, UIElement_C(glm::vec2(0.0f, 250.0f), glm::vec2(200.0f, 200.0f)));
    ui2->canvas = e2;

    current_scene->addComponent<Transform_C>(e4, Transform_C(e4));
    
    auto mat_font2 = std::dynamic_pointer_cast<MaterialFont>(c2->material);
    if (!mat_font2) {
        std::cout << "[ERROR] Failed to cast material to MaterialFont\n";
        return;
    }
    
    if (!mat_font2->font) {
        std::cout << "[ERROR] MaterialFont has no font loaded\n";
        return;
    }
    
    current_scene->addComponent<Text_C>(e3, Text_C(e3, mat_font1->font));
    
    current_scene->addComponent<Text_C>(e4, Text_C(e4, mat_font2->font));

    // Set alignment and rebuild mesh
    Text_C& text_c1 = current_scene->getComponent<Text_C>(e3);
    text_c1.alignment = TextAlign::CENTER;
    text_c1.v_alignment = VerticalAlign::CENTER;
    
    Text_C& text_c2 = current_scene->getComponent<Text_C>(e4);
    text_c2.alignment = TextAlign::CENTER;
    text_c2.v_alignment = VerticalAlign::CENTER;

    // Transform_C& t1 = current_scene->getComponent<Transform_C>(e3);
    // t1.position.x = -0.5f;

    // Transform_C& t2 = current_scene->getComponent<Transform_C>(e4);
    // t2.position.x = -0.5f;
    // t2.position.y = -0.3f;

    glfwSetKeyCallback(window, allingmentCallback);
    
    std::cout << "[LOG] SystemManager::initialize() complete!" << std::endl;
}

void SystemManager::update()
{
    if (!systems.empty()) systems[0]->update();
    
    // Transform_C* transform1 = &current_scene->getComponent<Transform_C>(e3);
    // Transform_C* transform2 = &current_scene->getComponent<Transform_C>(e2);

    
    // transform2->position.x = sin(-Time::time * 2.0);
    // transform2->position.z = cos(-Time::time * 2.0);
    
    // transform1->scale.x = (sin(Time::time) + 1.5);
    // transform1->scale.y = (sin(Time::time) + 1.5);
    
    // transform1->rotation.z = static_cast<float>(-Time::time);
    Text_C& t1 = current_scene->getComponent<Text_C>(e3);
    std::string txt1 = "Good day mister Juraj. Hello, how are you doing today?";
    t1.text_size = 16;
    std::string result1 = txt1.substr(0, std::round(((Time::sine + 1) * 0.5f) * txt1.length()));
    t1.text = result1;
    
    Text_C& t2 = current_scene->getComponent<Text_C>(e4);
    // t2.text = "Good day to you\nmister Mazda.";
    t2.text_size = 16;
    // std::string result2 = txt2.substr(0, std::round(((Time::cosine + 1) * 0.5f) * txt2.length()));
    // t2.text = result2;

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

