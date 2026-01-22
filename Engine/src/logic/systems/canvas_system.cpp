#include "canvas_system.h"
#include <logic/system_manager.h>
#include <engine_variables.h>

Canvas_S::Canvas_S() = default;
Canvas_S::~Canvas_S() = default;

void Canvas_S::update() {
    if (!SystemManager::current_scene) return;



    SystemManager::current_scene->forEach<Canvas_C>([&](Entity canvasEntity, Canvas_C& canvas) {
        glm::vec2 canvasCenter = glm::vec2(canvas.reference_width, canvas.reference_height) * 0.5f;
        
        // Scale from canvas coordinates to projection space
        // Projection is ortho(-aspect, aspect, -1, 1) based on screen resolution
        float screenAspect = static_cast<float>(resolution.x) / static_cast<float>(resolution.y);
        glm::vec2 scale = glm::vec2(screenAspect * 2.0f / canvas.reference_width, 2.0f / canvas.reference_height);

        SystemManager::current_scene->forEach<UIElement_C, Transform_C>(
            [&](Entity e, UIElement_C& ui, Transform_C& t) {

                if (ui.canvas != canvasEntity) return;

                // Convert from canvas space (0,0 = top-left) to world space (0,0 = center)
                // Y is inverted: canvas Y grows downward, world Y grows upward
                glm::vec2 offsetPos = glm::vec2(ui.position.x - canvasCenter.x, canvasCenter.y - ui.position.y);
                glm::vec2 finalPos = offsetPos * scale;
                glm::vec2 finalSize = ui.size * scale;

                t.position = {
                    finalPos.x,
                    finalPos.y,
                    0.0f
                };

                if (!ui.apply_scaling)
                    return;
                t.scale = {
                    finalSize.x,
                    finalSize.y,
                    1.0f
                };
            }
        );
    });
}