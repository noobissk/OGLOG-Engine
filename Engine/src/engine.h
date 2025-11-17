
#ifndef OGLOG_ENGINE
#define OGLOG_ENGINE
#include <config_system.h>
#include <condition_variable>
#include <logic/scene_manager.h>
#include <engine_variables.h>



class Engine
{
public:

    static GLFWwindow* window;


    static int startUp();
    static void quit();

    static unsigned int createModule(const std::string& filepath, unsigned int module_type);
    static unsigned int createShader(const std::string& vertex_module, const std::string& fragment_module);
};

#endif