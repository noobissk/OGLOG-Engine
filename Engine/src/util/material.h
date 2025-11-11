
#pragma once
#include <string>


class Material {
public:
    unsigned int texture;

    Material();
    void applyTexture(int unit);
    ~Material();
};