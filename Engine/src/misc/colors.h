#pragma once
#include <glm_math.h>



class Colors {
public:
    // static const glm::vec3 program_default  = { 0.4f, 0.37f, 0.55f };
    static const glm::vec3 program_default;

    static const glm::vec3 red;
    static const glm::vec3 green;
    static const glm::vec3 blue;
    static const glm::vec3 yellow;
    static const glm::vec3 orange;
    static const glm::vec3 purple;
    static const glm::vec3 pink;
    static const glm::vec3 cyan;
    static const glm::vec3 white;
    static const glm::vec3 black;
    static const glm::vec3 gray;
    static const glm::vec3 light_gray;

    static glm::vec3 FloatToRGB255(int r, int g, int b);

    static glm::vec3 RGB255Tofloat(int r, int g, int b);

};