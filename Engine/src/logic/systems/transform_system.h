#pragma once
#include <config_system.h>
#include <config_component.h>


class Transform_S : public System {
public:
    void awake() override {}
    void update() override;
    void updateComponents(Entity e, Transform_C& transform_c);
    Transform_S();
    ~Transform_S();

private:
    glm::mat4 computeLocalMatrix(const Transform_C& t);
};