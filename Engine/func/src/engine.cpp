#include <engine.h>
#include <logic/system_manager.h>
#include <misc/colors.h>

std::mutex Engine::m_mtx;
std::condition_variable Engine::m_cv;
bool Engine::m_isQuitTriggered = false;


int Engine::startUp()
{
    SystemManager::initialize();

    ScreenColor = Colors::program_default;
    glClearColor(ScreenColor.r, ScreenColor.g, ScreenColor.b, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    Engine::quit();

    return 0;
}


void Engine::waitUntilQuit() {
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cv.wait(lock, [] { return m_isQuitTriggered; });
}

void Engine::quit() {
    std::lock_guard<std::mutex> lock(m_mtx);
    m_isQuitTriggered = true;
    m_cv.notify_all();
}