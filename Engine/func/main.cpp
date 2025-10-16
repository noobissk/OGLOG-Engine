#include <engine.h>
#include <string>
#include <iostream>

static const std::string shaderPath = "../../func/shaders/";
static std::condition_variable cv;
std::mutex mtx;




int main ()
{
    if (!glfwInit())
    {
        return -1;
    }

    // glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(Resolution.x, Resolution.y, "OGLOG Engine project", NULL, NULL);

    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couln't load OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    Engine::startUp();

    Engine::waitUntilQuit();

    return 0;
}