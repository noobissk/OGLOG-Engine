
#pragma once
#include <vector>
#include <string>
#include "mesh.h"


class Model {
public:
    std::vector<Mesh> meshes;

    Model(const std::string& path);
};