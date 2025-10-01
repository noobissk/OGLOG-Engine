#include "scene_management.h"
#include <stdexcept>

// std::<Scene> SceneManager::all_scenes;



void SceneManager::loadScene(const std::string& name) {
    Scene& s = getSceneByName(name);
    active_scene = s;
}

void SceneManager::loadScene(const Scene& scene) {
    active_scene = scene;
}


void SceneManager::createScene(const std::string& name) {
    if (exists(name)) {
        std::string error_message = "Tried creating an Invalid scene. Scene with name: '" + name + "' already exists.";
        throw std::runtime_error(error_message);
    }
    all_scenes[name] = Scene(name);
}

void SceneManager::deleteScene(const std::string& name) {
    all_scenes.erase(name);
}

void SceneManager::deleteScene(const Scene& scene) {
    all_scenes.erase(scene.name);
}

// Scene& SceneManager::getSceneById(uint16_t index) {
//     return all_scenes[index];
// }


bool SceneManager::exists(const std::string& name) {
    return all_scenes.count(name) == 1;
}

bool SceneManager::exists(const Scene& scene) {
    return all_scenes.count(scene.name) == 1;
}
