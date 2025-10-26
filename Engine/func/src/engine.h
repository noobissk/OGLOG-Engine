
#ifndef OGLOG_ENGINE
#define OGLOG_ENGINE
#include <config_system.h>
#include <config_render.h>
#include <condition_variable>
#include <logic/scene_manager.h>


static const glm::vec3 right   (1.0f, 0.0f, 0.0f);
static const glm::vec3 up      (0.0f, 1.0f, 0.0f);
static const glm::vec3 forward (0.0f, 0.0f, 1.0f);

static glm::ivec2 resolution (840, 480);

static glm::vec3 screen_color (0.0f, 0.0f, 0.0f);

static const std::string shader_module_path = "../func/src/shaders/";




class Engine
{
public:
    static unsigned int shader;

    static GLFWwindow* window;


    static int startUp();
    static void quit();

    static unsigned int createModule(const std::string& filepath, unsigned int module_type);
    static unsigned int createShader(const std::string& vertex_module, const std::string& fragment_module);
};

#endif