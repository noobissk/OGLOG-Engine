#include "transform_system.h"
#include <logic/system_manager.h>
#include <glm/gtx/quaternion.hpp>

Transform_S::Transform_S() = default;
Transform_S::~Transform_S() = default;

void Transform_S::update() {
    if (!SystemManager::current_scene) return;
    SystemManager::current_scene->forEach<Transform_C>([this](Entity e, Transform_C& transform_c) {
        updateComponents(e, transform_c);
    });

}

void Transform_S::updateComponents(Entity e, Transform_C& transform_c) {
    Transform_C& t = transform_c;

    // Update local matrix
    t.localMatrix = computeLocalMatrix(t);

    // If no parent -> world = local
    if (t.parent == t.self) {
        t.worldMatrix = t.localMatrix;
        return;
    }

    // Combine with parents
    glm::mat4 world = t.localMatrix;
    Entity parent = t.parent;

    const Transform_C* p = nullptr;

    do {
        p = &SystemManager::current_scene->getComponent<Transform_C>(static_cast<Entity>(parent));
        world = p->localMatrix * world;
        parent = p->parent;
    } while (parent != p->self);

    t.worldMatrix = world;
}

glm::mat4 Transform_S::computeLocalMatrix(const Transform_C& t) {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), t.position);

    glm::mat4 rotation = 
      glm::rotate(glm::mat4(1.0f), t.rotation.y, {0,1,0})  // yaw
    * glm::rotate(glm::mat4(1.0f), t.rotation.x, {1,0,0})  // pitch
    * glm::rotate(glm::mat4(1.0f), t.rotation.z, {0,0,1}); // roll

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), t.scale);

    return translation * rotation * scale;
}