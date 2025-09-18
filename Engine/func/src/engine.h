
#pragma once
#include <glm_math.h>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include "logic/scene.h"
#include <stdexcept>


static const glm::vec3 Right   (1.0f, 0.0f, 0.0f);
static const glm::vec3 Up      (0.0f, 1.0f, 0.0f);
static const glm::vec3 Forward (0.0f, 0.0f, 1.0f);

static glm::ivec2 Resolution (840, 480);

static glm::vec3 ScreenColor (0.0f, 0.0f, 0.0f);

static GLFWwindow* window;


class SceneManager {
private:
    static std::vector<Scene> all_scenes;
    static Scene* active_scene;

public:
    static void loadScene(std::string name) {
        for (uint16_t i = 0; i < all_scenes.size(); i++)
        {
            if (all_scenes[i].name == name) {
                loadScene(all_scenes[i]);
                return;
            }
        }
        assert(false && "Scene with given name not found.");
    }
    
    static void loadScene(uint16_t index) {
        if (index >= all_scenes.size()) {
            assert(false && "Scene ID outside of given range (all_scenes)");
            return;
        }
        loadScene(all_scenes[index]);
    }

    static void loadScene(Scene scene) {
        active_scene = &scene;
    }

    static bool isActive(std::string sceneName) {
        return sceneName == active_scene->name;
    }
    static bool isActive(uint16_t sceneId) {
        if (sceneId > all_scenes.size()) {
            return false;
        }
        return all_scenes[sceneId].name == active_scene->name;
    }
    static bool isActive(Scene* scene) {
        return scene = active_scene;
    }
    
    static Scene& getSceneByID(uint16_t id) {
        if (id > all_scenes.size()) {
            throw std::runtime_error("Class not found: " + id);
        }
        return all_scenes[id];
    }

    static void createScene(std::string name) {
        for (Scene& s : all_scenes) {
            if (s.name == name) {
                return;
            }
        }
        all_scenes.emplace_back(name);
    }

    static void deleteScene() {
        
    }
};