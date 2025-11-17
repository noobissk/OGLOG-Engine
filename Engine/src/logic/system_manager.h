#pragma once
#include <config_system.h>
#include <logic/scene_manager.h>
#include <vector>

class SystemManager {
public:
    static std::vector<System*> systems;

    static Scene* current_scene;

    // std::vector<System> systems;

    static void initialize();

    static void awake();

    static void start();

    static void update();

    // static void fixedUpdate();

    // static void enable();

    // static void disable();
private:
    // Private constructor prevents instantiation
    SystemManager() = delete;
};