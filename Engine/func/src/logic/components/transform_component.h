
#pragma once
#include <logic/scene_manager.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
// #include <glm/gtx/quaternion.hpp>
#include <vector>

struct Transform_C {
    glm::vec3 position;
    glm::vec3 local_pos;

    glm::quat rotation;
    glm::quat local_rotation;

    Entity parent = 0;                   // 0 = no parent
    std::vector<Entity> children;

    inline glm::vec3 euler_angles() {
        return glm::eulerAngles(rotation);
    }
    inline glm::vec3 local_euler_angles() {
        return glm::eulerAngles(local_rotation); 
    }
};