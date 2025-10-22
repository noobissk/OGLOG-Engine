#include "scene_manager.h"
#include <stdexcept>
#include <logic/system_manager.h>

// In scene_manager.cpp
std::map<std::string, Scene> SceneManager::all_scenes {};

// Pick a default scene to reference, e.g. an empty "default" scene:
// Scene SceneManager::default_scene("Default");
std::string SceneManager::active_scene;




void SceneManager::loadScene(const std::string& name) {
    auto it = all_scenes.find(name);
    if (it == all_scenes.end()) throw std::runtime_error("Scene '" + name + "' not found");
    active_scene = name;
    SystemManager::current_scene = &it->second;
}

void SceneManager::loadScene(const Scene& scene) {
    auto it = all_scenes.find(scene.name);
    if (it == all_scenes.end()) throw std::runtime_error("Scene '" + scene.name + "' not found");
    active_scene = it->first;
    SystemManager::current_scene = &it->second;
}



void SceneManager::createScene(const std::string& name) {
    if (exists(name)) {
        throw std::runtime_error("Tried creating an invalid scene. Scene with name: '" + name + "' already exists.");
    }
    // construct in-place (no default ctor required)
    auto [it, inserted] = all_scenes.try_emplace(name, name);
    if (all_scenes.size() == 1) {
        // first scene becomes active
        active_scene = name;
        SystemManager::current_scene = &it->second;
    }
}

Scene& SceneManager::getSceneByName(const std::string& name)
{
    auto it = all_scenes.find(name);
    if (it == all_scenes.end()) {
        throw std::runtime_error("Scene '" + name + "' not found");
    }
    return it->second;
}

Scene& SceneManager::getActiveScene() {
    if (active_scene.empty()) throw std::runtime_error("No active scene set");
    return getSceneByName(active_scene); // avoids operator[]
}

void SceneManager::deleteScene(const std::string& name) {
    all_scenes.erase(name);
}

void SceneManager::deleteScene(const Scene& scene) {
    all_scenes.erase(scene.name);
}

bool SceneManager::exists(const std::string& name) {
    return all_scenes.count(name) == 1;
}

bool SceneManager::exists(const Scene& scene) {
    return all_scenes.count(scene.name) == 1;
}
