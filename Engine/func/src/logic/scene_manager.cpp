#include "scene_manager.h"
#include <stdexcept>

// In scene_manager.cpp
std::map<std::string, Scene> SceneManager::all_scenes {};

// Pick a default scene to reference, e.g. an empty "default" scene:
// Scene SceneManager::default_scene("Default");
std::string SceneManager::active_scene;


void SceneManager::loadScene(const std::string& name) {
    Scene& s = getSceneByName(name);
    active_scene = name;
}

void SceneManager::loadScene(const Scene& scene) {
    active_scene = scene.name;
}


void SceneManager::createScene(const std::string& name) {
    if (all_scenes.empty())
    {
        all_scenes.emplace(name, Scene(name));
        active_scene = name;
    }
    if (exists(name)) {
        std::string error_message = "Tried creating an Invalid scene. Scene with name: '" + name + "' already exists.";
        throw std::runtime_error(error_message);
    }
    // all_scenes[name] = Scene(name);
    all_scenes.emplace(name, Scene(name));
}

Scene& SceneManager::getSceneByName(const std::string& name)
{
    auto it = all_scenes.find(name);
    if (it == all_scenes.end()) {
        throw std::runtime_error("Scene '" + name + "' not found");
    }
    return it->second;
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
