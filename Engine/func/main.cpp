
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "engine.h"
#include "src/misc/colors.h"

static const std::string shaderPath = "../../func/shaders/";




int main ()
{
    if (!glfwInit())
    {
        return -1;
    }

    window = glfwCreateWindow(Resolution.x, Resolution.y, "OGLOG Engine project", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couln't load OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    ScreenColor = Colors::program_default;
    glClearColor(ScreenColor.r, ScreenColor.g, ScreenColor.b, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}