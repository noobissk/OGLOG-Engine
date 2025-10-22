#include <engine.h>
#include <logic/systems/systems.h>
#include <logic/system_manager.h>
#include <logic/scene_manager.h>
#include <misc/colors.h>

GLFWwindow* Engine::window = nullptr;

int Engine::startUp()
{
    SceneManager::createScene("default");
    SystemManager::initialize();

    ScreenColor = Colors::program_default;
    glClearColor(ScreenColor.r, ScreenColor.g, ScreenColor.b, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        SystemManager::update();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    Engine::quit();

    return 0;
}

void Engine::quit() { }