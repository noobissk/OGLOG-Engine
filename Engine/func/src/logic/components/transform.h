
#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

struct Transform {
    glm::vec3 position;
    glm::vec3 local_pos;

    glm::quat rotation;
    glm::quat local_rotation;

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    inline glm::vec3 euler_angles() {
        return glm::eulerAngles(rotation);
    }
    inline glm::vec3 local_euler_angles() {
        return glm::eulerAngles(local_rotation); 
    }
};