#include <glm/glm.hpp>
using Entity = uint64_t;

struct UIElement_C {
public:
    Entity canvas;

    bool apply_scaling = true;

    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 pivot = {0.0f, 0.0f};
    glm::vec2 anchor = {0.0f, 0.0f};;
    
    UIElement_C() : position(0.0f), size(100.0f, 50.0f) {}
    UIElement_C(const glm::vec2& _pos, const glm::vec2& _size) : position(_pos), size(_size) {}
};