#ifndef SCENE_MANAGEMENT
#define SCENE_MANAGEMENT


#include <string>
#include <vector>
#include "logic/scene.h"
#include <stdexcept>
#include <map>

class SceneManager {
private:
    static std::map<std::string, Scene> all_scenes;

public:
    static std::string active_scene;

    unsigned int scene_count = all_scenes.size();

    static void loadScene(const std::string& name);
    static void loadScene(const Scene& scene);

    static Scene& getSceneByName(const std::string& name);
    static Scene& getActiveScene();

    static void createScene(const std::string& name);

    static void deleteScene(const std::string& name);
    static void deleteScene(const Scene& scene);

    static bool exists(const std::string& name);
    static bool exists(const Scene& scene);
};

#endif