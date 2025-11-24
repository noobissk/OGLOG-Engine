
#pragma once
#include <logic/scene_manager.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

struct Transform_C {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 localMatrix;
    glm::mat4 worldMatrix;

    int16_t parent = -1;  // -1 = no parent

    // Don't query GL uniforms in the component constructor (unsafe if context/shader isn't ready).
    Transform_C() : position(0.0f, 0.0f, 0.0f),
                    rotation(0.0f, 0.0f, 0.0f),
                    scale(1.0f, 1.0f, 1.0f),
                    localMatrix(1.0f),
                    worldMatrix(1.0f),
                    parent(-1) {}
};