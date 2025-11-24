
#pragma once
#include <logic/scene_manager.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

using Entity = uint64_t;

struct Transform_C {
    Entity self;  // Initialize first
    Entity parent;  // self = no parent

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 localMatrix;
    glm::mat4 worldMatrix;

    // Don't query GL uniforms in the component constructor (unsafe if context/shader isn't ready).
    // Initialize self first, then parent defaults to self (no parent).
    Transform_C(Entity self_) : self(self_),
                    parent(self_),
                    position(0.0f, 0.0f, 0.0f),
                    rotation(0.0f, 0.0f, 0.0f),
                    scale(1.0f, 1.0f, 1.0f),
                    localMatrix(1.0f),
                    worldMatrix(1.0f) {}
};