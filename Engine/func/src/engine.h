
#ifndef OGLOG_ENGINE
#define OGLOG_ENGINE
#include <glm_math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <condition_variable>
#include <logic/scene_manager.h>


static const glm::vec3 Right   (1.0f, 0.0f, 0.0f);
static const glm::vec3 Up      (0.0f, 1.0f, 0.0f);
static const glm::vec3 Forward (0.0f, 0.0f, 1.0f);

static glm::ivec2 Resolution (840, 480);

static glm::vec3 ScreenColor (0.0f, 0.0f, 0.0f);




class Engine
{
public:
    static GLFWwindow* window;
    static int startUp();

    static void quit();
};

#endif