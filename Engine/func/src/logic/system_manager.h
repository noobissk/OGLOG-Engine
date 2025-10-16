#pragma once
#include <vector>
#include <logic/system.h>

class SystemManager {
public:
    // std::vector<System> systems;

    static void initialize();

    static void awake();

    static void start();

    // static void update();

    // static void fixedUpdate();

    // static void enable();

    // static void disable();
private:
    // Private constructor prevents instantiation
    SystemManager() = delete;
};