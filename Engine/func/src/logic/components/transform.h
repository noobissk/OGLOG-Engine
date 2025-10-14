
#pragma once
#include <gml/vector.h>
#include <gml/quaternion.h>
#include <vector>

struct Transform {
    gml::vec3 position;
    gml::vec3 local_pos;

    gml::quat rotation;
    gml::quat local_rotation;

    Transform* parent;
    std::vector<Transform*> children;

    inline gml::vec3 euler_angles() {
        return gml::to_euler(rotation);
    }
    inline gml::vec3 local_euler_angles() {
        return gml::to_euler(local_rotation); 
    }
}