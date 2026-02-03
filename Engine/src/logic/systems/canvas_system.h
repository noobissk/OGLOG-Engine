#pragma once
#include <config_system.h>
#include <config_component.h>


class Canvas_S : public System {
private:
    bool mouse_clicked = false;
    static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
public:
    glm::dvec2 mousePos;
    void awake() override;
    void update() override;
    Canvas_S();
    ~Canvas_S();
};