
#ifndef OGLOG_ENGINE
#define OGLOG_ENGINE
#include <config_system.h>
#include <condition_variable>
#include <logic/scene_manager.h>
#include <engine_variables.h>



class Engine
{
public:
    static int startUp();
    static void quit();
    static void frameBufferSizeCallback(int width, int height);
};

#endif