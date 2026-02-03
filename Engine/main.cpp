#define STB_IMAGE_IMPLEMENTATION
#include <config_render.h>
#include <engine.h>
#include <string>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Engine::frameBufferSizeCallback(width, height);
}

int main ()
{
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(resolution.x, resolution.y, "OGLOG Engine project", NULL, NULL);

    if (!window)
    {
        std::cout << "[ERROR] Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "[ERROR] Couln't load OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Engine::startUp();
    return 0;
}