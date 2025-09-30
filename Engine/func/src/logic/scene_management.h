// #pragma once
// #include <string>
// #include <vector>
// #include "logic/scene.h"
// #include <stdexcept>

// class SceneManager {
// private:
//     static std::vector<Scene> all_scenes;

// public:
//     static Scene* active_scene;

//     unsigned int scene_count = all_scenes.size();


//     static void loadScene(std::string name) {
//         for (uint16_t i = 0; i < all_scenes.size(); i++)
//         {
//             if (all_scenes[i].name == name) {
//                 loadScene(all_scenes[i]);
//                 return;
//             }
//         }
//         assert(false && "Scene with given name not found.");
//     }
    
//     static void loadScene(uint16_t index) {
//         if (index >= all_scenes.size()) {
//             assert(false && "Scene ID outside of given range (all_scenes)");
//             return;
//         }
//         loadScene(all_scenes[index]);
//     }

//     static void loadScene(Scene scene) {
//         active_scene = &scene;
//     }

//     static bool isActive(std::string sceneName) {
//         return sceneName == active_scene->name;
//     }
//     static bool isActive(uint16_t sceneId) {
//         if (sceneId >= all_scenes.size()) {
//             return false;
//         }
//         return all_scenes[sceneId].name == active_scene->name;
//     }
//     static bool isActive(Scene* scene) {
//         return scene = active_scene;
//     }
    
//     static Scene& getSceneByID(uint16_t id) {
//         if (id >= all_scenes.size()) {
//             throw std::runtime_error("Scene not found: " + id);
//         }
//         return all_scenes[id];
//     }
//     static Scene& getSceneByName(std::string& name) {
//         for (Scene& s : all_scenes) {
//             if (s.name == name) {
//                 return s;
//             }
//         }
//         throw std::runtime_error("Scene not found: " + name);
//     }

//     static void createScene(std::string name) {
//         all_scenes.emplace_back(getSceneByName(name));
//     }

//     static void deleteScene(uint16_t id) {
//         if (id >= all_scenes.size()) {
//             throw std::runtime_error("Failed to delete scene " + id);
//         }
//         all_scenes.erase(all_scenes.begin() + id);
//     }
//     static void deleteScene(std::string& name) {
//         all_scenes.erase(
//             std::remove(all_scenes.begin(), all_scenes.end(), getSceneByName(name)),
//             all_scenes.end()
//         );
//     }
//     static void deleteScene(const Scene& scene) {
//         all_scenes.erase(
//             std::remove(all_scenes.begin(), all_scenes.end(), scene),
//             all_scenes.end()
//         );
//     }
// };