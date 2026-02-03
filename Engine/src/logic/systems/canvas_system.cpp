#include "canvas_system.h"
#include <logic/system_manager.h>
#include <config_render.h>
#include <engine_variables.h>

Canvas_S::Canvas_S() = default;
Canvas_S::~Canvas_S() = default;


void Canvas_S::mouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        auto* self = static_cast<Canvas_S*>(glfwGetWindowUserPointer(window));
        if (self)
            self->mouse_clicked = true;
    }
}

void Canvas_S::awake() {
    glfwSetWindowUserPointer(window, this);
    glfwSetMouseButtonCallback(window, mouseClickCallback);
}

void Canvas_S::update() {
    if (!SystemManager::current_scene) return;



    SystemManager::current_scene->forEach<Canvas_C>([&](Entity canvasEntity, Canvas_C& canvas) {
        glm::vec2 canvasCenter = glm::vec2(canvas.reference_width, canvas.reference_height) * 0.5f;
        
        // Scale from canvas coordinates to projection space
        // Projection is ortho(-aspect, aspect, -1, 1) based on screen resolution
        float screenAspect = static_cast<float>(resolution.x) / static_cast<float>(resolution.y);
        
        canvas.cursor_scale = std::min(
            resolution.x / (float)canvas.reference_width,
            resolution.y / (float)canvas.reference_height
        );
        canvas.actual_pixel_width = canvas.reference_width * canvas.cursor_scale;
        canvas.actual_pixel_height = canvas.reference_height * canvas.cursor_scale;

        canvas.offset_x = (resolution.x - canvas.actual_pixel_width) / 2.0f;
        canvas.offset_y = (resolution.y - canvas.actual_pixel_height) / 2.0f;

        SystemManager::current_scene->forEach<UIElement_C, Transform_C>(
            [&](Entity e, UIElement_C& ui, Transform_C& t) {
                if (ui.canvas != canvasEntity) return;

                float sx = (2.0f * screenAspect) / canvas.reference_width;
                float sy = 2.0f / canvas.reference_height;

                // Pick ONE scale
                float canvasScale = std::min(sx, sy);   // letterbox-safez

                glm::vec2 offsetPos = glm::vec2(ui.position.x - canvasCenter.x, canvasCenter.y - ui.position.y);
                glm::vec2 finalPos  = offsetPos * canvasScale;
                glm::vec2 finalSize = ui.size * canvasScale;

                t.position = { finalPos.x, finalPos.y, 0.0f };

                if (!ui.apply_scaling)
                    return;
                if (ui.apply_scaling) {
                    t.scale = { finalSize.x, finalSize.y, 1.0f };
                }
            }
        );
    });
    
    SystemManager::current_scene->forEach<Button_C, UIElement_C, MeshRenderer_C>([&](Entity e, Button_C& button, UIElement_C& ui, MeshRenderer_C& mesh) {
            glfwGetCursorPos(window, &mousePos.x, &mousePos.y);

            mousePos.y = static_cast<float>(resolution.y) - mousePos.y;
            glm::vec2 canvasMousePos = mousePos;
            SystemManager::current_scene->forEach<Canvas_C>([&](Entity canvasEntity, Canvas_C& canvas) {
                if (ui.canvas == canvasEntity) {
                    canvasMousePos.x = (mousePos.x - canvas.offset_x) / canvas.cursor_scale;
                    canvasMousePos.y = canvas.reference_height - (mousePos.y - canvas.offset_y) / canvas.cursor_scale;  // flip Y
                };
            });

            float uiHalfX = ui.size.x * 0.5f;
            float uiHalfY = ui.size.y * 0.5f;
            if (canvasMousePos.x >= ui.position.x - uiHalfX &&
                canvasMousePos.x <= ui.position.x + uiHalfX &&
                canvasMousePos.y >= ui.position.y - uiHalfY &&
                canvasMousePos.y <= ui.position.y + uiHalfY) {

                if (auto mat = std::dynamic_pointer_cast<MaterialDefault>(mesh.material)) {
                    mat->color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
                }

                if (mouse_clicked && button.on_click) {
                    button.on_click();
                }
            }
            else {
                if (auto mat = std::dynamic_pointer_cast<MaterialDefault>(mesh.material)) {
                    mat->color = glm::vec4(1.0f);
                }
            }
        }
    );
    mouse_clicked = false;
}