#include "colors.h"

const glm::vec3 Colors::program_default = glm::vec3(0.188235294f, 0.043137255f, 0.145098039f);
const glm::vec3 Colors::red             = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Colors::green           = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Colors::blue            = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 Colors::yellow          = glm::vec3(1.0f, 1.0f, 0.0f);
const glm::vec3 Colors::orange          = glm::vec3(1.0f, 0.65f, 0.0f);
const glm::vec3 Colors::purple          = glm::vec3(0.5f, 0.0f, 0.5f);
const glm::vec3 Colors::pink            = glm::vec3(1.0f, 0.75f, 0.8f);
const glm::vec3 Colors::cyan            = glm::vec3(0.0f, 1.0f, 1.0f);
const glm::vec3 Colors::white           = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 Colors::black           = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 Colors::gray            = glm::vec3(0.5f, 0.5f, 0.5f);
const glm::vec3 Colors::light_gray      = glm::vec3(0.83f, 0.83f, 0.83f);


glm::vec3 Colors::FloatToRGB255(int r, int g, int b) {
    return glm::vec3(r/255.0f, g/255.0f, b/255.0f);
}
glm::vec3 Colors::RGB255Tofloat(int r, int g, int b) {
    return glm::vec3(r*255.0f, g*255.0f, b*255.0f);
}